#include <mqueue.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>


#define MAXSIZE 256
#define TRUE 1
#define FALSE 0

struct request{
    char key[MAXSIZE];
    char value1[MAXSIZE];
    float value2;
    char q_name[MAXSIZE];
};

pthread_mutex_t mutex_msg;
int msg_not_copied = TRUE;
pthread_cond_t cond_msg;

void process_message(struct request *msg){
    struct request msg_local;
    mqd_t q_client;
    int result;

    pthread_mutex_lock(&mutex_msg);
    memcpy((char *) &msg_local, (char *) &msg, sizeof(struct request));

    msg_not_copied = FALSE;

    pthread_cond_signal(&cond_msg);

    pthread_mutex_unlock(&mutex_msg);

    result = 0;

    q_client = mq_open(msg_local.q_name, O_WRONLY);

    if(q_client == -1)
        perror("Can't open client queue");
    else{
        mq_send(q_client, (char *) &result, sizeof(int), 0);
        mq_close(q_client);
    }
    pthread_exit(0);
}

int main(void){
    mqd_t q_server;
    struct request msg;
    struct mq_attr q_attr;
    pthread_attr_t t_attr;
    pthread_t thid;

    q_attr.mq_maxmsg = 20;
    q_attr.mq_msgsize = sizeof(struct request);

    q_server = mq_open("SERVER", O_CREAT|O_RDONLY, 0700, &q_attr);
    if(q_server == -1){
        perror("Can't create server queue");
        return 1;
    }

    pthread_mutex_init(&mutex_msg, NULL);
    pthread_cond_init(&cond_msg, NULL);
    pthread_attr_init(&t_attr);

    pthread_attr_setdetachstate(&t_attr, PTHREAD_CREATE_DETACHED);

    while(TRUE){
        mq_receive(q_server, (char *) &msg, sizeof(struct request), 0);
        pthread_create(&thid, &t_attr, (void (*))process_message, &msg);

        pthread_mutex_lock(&mutex_msg);
        while(msg_not_copied)
            pthread_cond_wait(&cond_msg, &mutex_msg);
        msg_not_copied = TRUE;
        pthread_mutex_unlock(&mutex_msg);
    }
}