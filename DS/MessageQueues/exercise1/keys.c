#include "keys.h"

int init()
{
  int res;


  Triplets triplet;

  mq_close(q_client);
  mq_close(q_server);

  attributes_s.mq_maxmsg  = MAX_MESSAGES;
  attributes_s.mq_msgsize = sizeof(triplet);
  attributes_c.mq_maxmsg  = MAX_MESSAGES;
  attributes_c.mq_msgsize = sizeof(int);


  if ((q_client = mq_open(CLIENT_QUEUE, O_CREAT|O_RDWR, 0777, &attributes_c))==-1)
  {
    printf("Error mq_open\n");
    exit(-1);
  } else
  {
    printf("CLIENT QUEUE OPENED (CLIENT SIDE_1)\n");
  }

  if ((q_server = mq_open(SERVER_QUEUE, O_WRONLY, 0777, &attributes_s))==-1)
  {
   perror("Error mq_open\n");
   exit(-1);
 } else
 {
   printf("SERVER QUEUE OPENED (CLIENT SIDE_1)\n");
 }

 strcpy( triplet.q_name, CLIENT_QUEUE);
 triplet.op_code = 1;
 bzero(triplet.key, MAX_SIZE);

 if (mq_send(q_server, (const char *) &triplet, sizeof(Triplets), 0)== -1)
 {
  perror("mq_send");
  printf("%s", triplet.key);
  mq_close(q_server);
  exit(1);
}

if (mq_receive(q_client, (char *) &res, sizeof(int), 0)== -1)
{
  perror("mq_recv");
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

  

  attributes_s.mq_maxmsg  = MAX_MESSAGES;
  attributes_s.mq_msgsize = sizeof(triplet);

  if ((q_server = mq_open(SERVER_QUEUE, O_WRONLY, 0777, &attributes_s))==-1)
  {
    perror("Error mq_open\n");
    exit(-1);
  } else
  {
    printf("SERVER QUEUE OPENED (CLIENT SIDE_2)\n");
  }

  if (mq_send(q_server, (const char *) &triplet, sizeof(Triplets), 0)== -1)
  {
    perror("mq_send");
    printf("%s", triplet.key);
    mq_close(q_server);
    exit(1);
  } else
  {
    printf("MESSAGE SENT FROM CLIENT TO SERVER (CLIENT SIDE_2)\n");
  }

  mq_close(q_server);

  return 0;
}

int get_value(char* key, char *value1, float *value2)
{

  Triplets triplet;

  attributes_s.mq_maxmsg  = MAX_MESSAGES;
  attributes_s.mq_msgsize = sizeof(triplet);

  strcpy( triplet.key, key);
  //strcpy( triplet.value1, value1); 
  //triplet.value2 = *value2;
  triplet.op_code = 3;
  strcpy( triplet.q_name, CLIENT_QUEUE);


  if ((q_server = mq_open(SERVER_QUEUE, O_WRONLY, 0777, &attributes_s))==-1)
  {
    perror("Error mq_open\n");
    exit(-1);
  } else
  {
    printf("SERVER QUEUE OPENED (CLIENT SIDE_3)\n");
  }

  if ((q_client = mq_open(CLIENT_QUEUE, O_RDONLY, 0777, &attributes_s))==-1)
  {
    perror("Error mq_open\n");
    exit(-1);
  } else
  {
    printf("CLIENT QUEUE OPENED (CLIENT SIDE_3)\n");
  }

  if (mq_send(q_server, (const char *) &triplet, sizeof(Triplets), 0)== -1)
  {
    perror("mq_send");

    mq_close(q_server);
    exit(1);
  } else
  {
    printf("MESSAGE SENT FROM CLIENT TO SERVER (CLIENT SIDE_3)\n");
  }

  if((mq_receive(q_client, (char *) &triplet, sizeof(Triplets), 0)==-1))
  {
    perror("ERROR (CLIENT SIDE_3) -> mq_receive");
    mq_close(q_client);
    exit(1);
  } else
  {
    printf("RECEIVED KEY %s\n", triplet.key );
    printf("RECEIVED VALUE_1 %s\n", triplet.value1);
    printf("RECEIVED VALUE_2 %f\n", triplet.value2);

  }


  mq_close(q_client);
  mq_close(q_server);


  return 0;
}

int modify_value(char* key, char *value1, float *value2)
{
//	int mq_setattr(mqd_t mqdes, struct mq_attr *qstat,struct mq_attr *oldmqstat);
  return 0;
}

int delete_key(char* key)
{
  // Triplets triplet;
  
  // strcpy( triplet.key, key);
  
  // triplet.op_code = 5;
  // strcpy( triplet.q_name, CLIENT_QUEUE);

  // attributes_s.mq_maxmsg  = MAX_MESSAGES;
  // attributes_s.mq_msgsize = sizeof(triplet);

  // if ((q_server = mq_open(SERVER_QUEUE, O_WRONLY, 0777, &attributes_s))==-1)
  // {
  //   perror("Error mq_open\n");
  //   exit(-1);
  // } else
  // {
  //   printf("SERVER QUEUE OPENED (CLIENT SIDE_2)\n");
  // }
  
  // if (mq_send(q_server, (const char *) &triplet, sizeof(Triplets), 0)== -1)
  // {
  //   perror("mq_send");
  //   printf("%s", triplet.key);
  //   mq_close(q_server);
  //   exit(1);
  // } else
  // {
  //   printf("MESSAGE SENT FROM CLIENT TO SERVER (CLIENT SIDE_2)\n");
  // }

  // mq_close(q_server);

  return 0;
}

int exist(char* key)
{
  Triplets triplet;

  attributes_s.mq_maxmsg  = MAX_MESSAGES;
  attributes_s.mq_msgsize = sizeof(triplet);
  strcpy( triplet.key, key);
  strcpy( triplet.q_name, CLIENT_QUEUE);
  triplet.op_code = 6;

  int exists;

  if ((q_server = mq_open(SERVER_QUEUE, O_WRONLY, 0777, &attributes_s))==-1)
  {
    perror("Error mq_open\n");
    exit(-1);
  } else
  {
    printf("SERVER QUEUE OPENED (CLIENT SIDE_2)\n");
  }

  if ((q_client = mq_open(CLIENT_QUEUE, O_RDONLY, 0777, &attributes_s))==-1)
  {
    perror("Error mq_open\n");
    exit(-1);
  } else
  {
    printf("CLIENT QUEUE OPENED (CLIENT SIDE_3)\n");
  }

  if (mq_send(q_server, (const char *) &triplet, sizeof(Triplets), 0)== -1)
  {
    perror("mq_send");

    mq_close(q_server);
    exit(1);
  } else
  {
    printf("MESSAGE SENT FROM CLIENT TO SERVER (CLIENT SIDE_3)\n");
  }

  if((mq_receive(q_client, (char *) &exists, sizeof(int), 0)==-1))
  {
    perror("ERROR (CLIENT SIDE_6) -> mq_receive");
    mq_close(q_client);
    exit(1);
  } else
  {
    if(exists == 1)
    {
      printf("ELEMENT EXISTS\n");
    } else 
    {
      printf("ELEMENT DOESN'T EXIST\n");
    }

  }


  mq_close(q_client);
  mq_close(q_server);

  return 0;
}

int num_items()
{


  Triplets triplet;

  attributes_s.mq_maxmsg  = MAX_MESSAGES;
  attributes_s.mq_msgsize = sizeof(triplet);
  strcpy( triplet.q_name, CLIENT_QUEUE);
  triplet.op_code = 7;

  int count;

  if ((q_server = mq_open(SERVER_QUEUE, O_WRONLY, 0777, &attributes_s))==-1)
  {
    perror("Error mq_open\n");
    exit(-1);
  } else
  {
    printf("SERVER QUEUE OPENED (CLIENT SIDE_2)\n");
  }

  if ((q_client = mq_open(CLIENT_QUEUE, O_RDONLY, 0777, &attributes_s))==-1)
  {
    perror("Error mq_open\n");
    exit(-1);
  } else
  {
    printf("CLIENT QUEUE OPENED (CLIENT SIDE_3)\n");
  }

  if (mq_send(q_server, (const char *) &triplet, sizeof(Triplets), 0)== -1)
  {
    perror("mq_send");

    mq_close(q_server);
    exit(1);
  } else
  {
    printf("MESSAGE SENT FROM CLIENT TO SERVER (CLIENT SIDE_3)\n");
  }

  if((mq_receive(q_client, (char *) &count, sizeof(int), 0)==-1))
  {
    perror("ERROR (CLIENT SIDE_3) -> mq_receive");
    mq_close(q_client);
    exit(1);
  } else
  {
    printf("NUMBER OF ELEMENTS IN QUEUE : %d\n", count );
  }


  mq_close(q_client);
  mq_close(q_server);


  return 0;
}
