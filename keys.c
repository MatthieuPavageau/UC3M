#include "keys.h"
#define MAX_SIZE 256
#define QUEUE_PERMISSIONS 0660
#define MAX_MESSAGES 10
//#define MSG_BUFFER_SIZE MAX_SIZE
#define SERVER_QUEUE "/SERVER_QUEUE"
#define CLIENT_QUEUE "/CLIENT_QUEUE"




typedef struct Triplets{
  char key[MAX_SIZE];
  char value1[MAX_SIZE];
  float value2;
  char q_name[MAX_SIZE];
  int op_code;
} Triplets;

 mqd_t q_client;
 mqd_t q_server;

int init()
{
    int res;
   
    Triplets triplet;

    mq_close(q_client);
    mq_close(q_server);

    struct mq_attr attributes_c;
   	struct mq_attr attributes_s;

   	attributes_s.mq_maxmsg  = MAX_MESSAGES;
    attributes_s.mq_msgsize = sizeof(triplet);
    attributes_c.mq_maxmsg  = MAX_MESSAGES;
    attributes_c.mq_msgsize = sizeof(int);


    if ((q_client = mq_open(CLIENT_QUEUE, O_CREAT|O_RDONLY, 0777, &attributes_c))==-1)
    {
      printf("AAA Error mq_open\n");
      exit(-1);
    } else
    {
      printf("CLIENT QUEUE OPENED\n");
    }

   	if ((q_server = mq_open(SERVER_QUEUE, O_WRONLY, 0777, &attributes_s))==-1)
   	{
   		perror(" BBB Error mq_open\n");
   		exit(-1);
   	} else
   	{
   		printf("SERVER QUEUE OPENED\n");
   	}

    strcpy( triplet.q_name, CLIENT_QUEUE);
    triplet.op_code = 1;
    bzero(triplet.key, MAX_SIZE);

    if (mq_send(q_server, (const char *) &triplet, sizeof(Triplets), 0)== -1)
    {
      perror(" CCC mq_send");
          printf("%s", triplet.key);
          mq_close(q_server);
      exit(1);
    }

    if (mq_receive(q_client, (char *) &res, sizeof(int), 0)== -1)
    {
      perror(" DDD mq_recv");
       printf("%s", triplet.key);
      mq_close(q_server);
      exit(1);
    } else
    {
      printf("%d\n", res);
    }
    
    
    mq_close(q_client);
    mq_close(q_server);

    return 0;
}

int set_value(char* key, char* value1, float value2)
{
  
  Triplets triplet;
  
  strcpy( triplet.key, key);
  strcpy( triplet.value1, value1); 
  triplet.value2 = value2;
  triplet.op_code = 2;
  strcpy( triplet.q_name, CLIENT_QUEUE);

  struct mq_attr attributes_s;

  attributes_s.mq_maxmsg  = MAX_MESSAGES;
  attributes_s.mq_msgsize = sizeof(triplet);

  if ((q_server = mq_open(SERVER_QUEUE, O_WRONLY, 0777, &attributes_s))==-1)
    {
      perror(" BBB Error mq_open\n");
      exit(-1);
    } else
    {
      printf("SERVER QUEUE OPENED\n");
    }
 
    if (mq_send(q_server, (const char *) &triplet, sizeof(Triplets), 0)== -1)
    {
      perror("mq_send");
       printf("%s", triplet.key);
      mq_close(q_server);
      exit(1);
    } else
    {
      printf("MESSAGE SENT FROM CLIENT TO SERVER");
    }

  mq_close(q_server);
  		
    return 0;
}

int get_value(char* key, char *value1, float *value2)
{
//	int mq_getattr (mqd_t mqdes, struct mq_attr *qstat)
    return 0;
}

int modify_value(char* key, char *value1, float *value2)
{
//	int mq_setattr(mqd_t mqdes, struct mq_attr *qstat,struct mq_attr *oldmqstat);
    return 0;
}

int delete_key(char* key)
{
  // RECEIVE and set to null
    return 0;
}

int exist(char* key)
{
    //  int mq_getattr (mqd_t mqdes, struct mq_attr *qstat)
    // 0 yes exist -1 no
    return 0;
}

int num_items()
{

    return 0;
}

