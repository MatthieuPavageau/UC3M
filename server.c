#include <mqueue.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include "keys.h"

#define MAX_MESSAGES 10
#define MAX_SIZE 256
#define TRUE 1
#define FALSE 0
#define SERVER_QUEUE "/SERVER_QUEUE"
#define CLIENT_QUEUE "/CLIENT_QUEUE"


typedef struct Triplets
{
  char key[MAX_SIZE];
  char value1[MAX_SIZE];
  float value2;
  char q_name[MAX_SIZE];
  int op_code;
} Triplets;

pthread_mutex_t mutex_msg;
int msg_not_copied = TRUE;
pthread_cond_t cond_msg;
int val;

mqd_t q_server;
mqd_t q_client;

void process_message(Triplets *msg){
    Triplets triplet;
    mqd_t q_client;
    int result;

    struct mq_attr q_attr;

    q_attr.mq_maxmsg = 10;
    q_attr.mq_msgsize = sizeof(Triplets);

    pthread_mutex_lock(&mutex_msg);
    memcpy((char *) &triplet, (char *) msg, sizeof(Triplets));

    msg_not_copied = FALSE;

    pthread_cond_signal(&cond_msg);

    pthread_mutex_unlock(&mutex_msg);

    if((q_client = mq_open(triplet.q_name, O_RDWR,0777,&q_attr))==-1)
   		{
    		    	perror("AAA Can't open client queue");
    		    	pthread_exit(NULL);
        }

    switch(triplet.op_code)
    {
    	case 1:
    		 printf("SYSTEMT INITIALIZATION ... \n");
    		 result = 1;
    		 if((mq_send(q_client, (char *) &result, sizeof(int), 0)==-1))
    		 {
    		 	perror("1) mq_send\n");
    		 	mq_close(q_client);
    		 } else {
    		 	printf("SYSTEM INITIALIZED\n");
    		 }
    		 	mq_close(q_client);
    		break;
    	case 2:
    		 if((mq_receive(q_client, (char *) &triplet, sizeof(Triplets), 0)==-1))
    		 {
    		 	perror("2) mq_receive\n");
    		 } else {
    		 	printf("VALUE SET\n");
    		 	printf("ASASDASD %f\n", triplet.value2 );
    		 }



    		 	mq_close(q_client);
    		break;
    	case 3:
    		break;
    	case 4:
    		break;
    	case 5:
    	   	break;
    	case 6:
    		break;
    	case 7:
    		break;
    }
   
    mq_close(q_client);

    
    pthread_exit(NULL);
}

int main(void)
{
    mqd_t q_server;
    
    Triplets msg;

    struct mq_attr q_attr;
    pthread_attr_t t_attr;
    pthread_t thid;

  	q_attr.mq_maxmsg = 10;
    q_attr.mq_msgsize = sizeof(Triplets);

    //mq_unlink(SERVER_QUEUE);

    if((q_server = mq_open(SERVER_QUEUE, O_CREAT|O_RDWR, 0777, &q_attr))==-1)
    {
    		perror("Can't create server queue");
        	return 1;
   	}
  
    pthread_mutex_init(&mutex_msg, NULL);
    pthread_cond_init(&cond_msg, NULL);
    pthread_attr_init(&t_attr);

    pthread_attr_setdetachstate(&t_attr, PTHREAD_CREATE_DETACHED);

    while(TRUE)
    {
        if((mq_receive(q_server, (char *) &msg, sizeof(Triplets), 0)==-1))
        {
        	perror("ERROR (SERVER SIDE) -> mq_receive");
        } else
        {
        	  printf("KEY RECEIVED AT SERVER SIDE            : %s\n",msg.key);
        	  printf("VALUE 1 RECEIVED AT SERVER SIDE        : %s\n",msg.value1);
        	  printf("VALUE 2 RECEIVED AT SERVER SIDE        : %f\n",msg.value2);
              printf("QUEUE NAME RECEIVED AT SERVER SIDE     : %s\n",msg.q_name);
              printf("OPERATION CODE RECEIVED AT SERVER SIDE : %d\n",msg.op_code);
        }
      
        pthread_create(&thid, &t_attr, (void (*))process_message, &msg);
        pthread_mutex_lock(&mutex_msg);
        while(msg_not_copied)
            pthread_cond_wait(&cond_msg, &mutex_msg);
        msg_not_copied = TRUE;
        pthread_mutex_unlock(&mutex_msg);
    }
}
