#include <mqueue.h>
#include <pthread.h>
#include <stdio.h>

#define MAXSIZE 256
#define TRUE 1

struct request{
    char key[MAXSIZE];
    char value1[MAXSIZE];
    float value2;
    char q_name[MAXSIZE];
};

pthread_mutex_t mutex_msg;
int msg_not_copied = TRUE;
pthread_cond_t cond_msg;

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
        perror("Can't cerate server queue");
        return 1;
    }

    pthread_mutex_init(&mutex_msg, NULL);
    pthread_cond_init(&cond_msg, NULL);
    pthread_attr_init(&q_attr);

    pthread_attr_setdetachstate(&t_attr, PTHREAD_CREATE_DETACHED);

    while(TRUE){
        mq_receive(q_server, &msg, sizeof(struct request), 0);
        pthread_create(&thid, &t_attr, process_message, &msg);

        pthread_mutex_lock(&mutex_msg);
        while(msg_not_copied)
            pthread_cond_wait(&cond_msg, &mutex_msg);
        msg_not_copied = TRUE;
        pthread_mutex_unlock(&mutex_msg);
    }
}

void process_message(struct request *msg){
    
}