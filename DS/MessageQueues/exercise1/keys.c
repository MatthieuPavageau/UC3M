#include "keys.h"

int init(){
    Answers res;
    Triplets triplet;

    attributes_c.mq_maxmsg  = MAX_MESSAGES;
    attributes_c.mq_msgsize = sizeof(Answers);
    attributes_s.mq_maxmsg  = MAX_MESSAGES;
    attributes_s.mq_msgsize = sizeof(Triplets);

    if((q_client = mq_open(CLIENT_QUEUE, O_CREAT|O_RDWR, 0777, &attributes_c)) == -1){
        printf("Error mq_open\n");
        exit(-1);
    } else{
        printf("CLIENT QUEUE OPENED (CLIENT SIDE_1)\n");
    }

    if((q_server = mq_open(SERVER_QUEUE, O_WRONLY, 0777, &attributes_s)) == -1){
        perror("Error mq_open\n");
        exit(-1);
   	} else{
   		  printf("SERVER QUEUE OPENED (CLIENT SIDE_1)\n");
   	}
    
   	strcpy(triplet.q_name, CLIENT_QUEUE);
    triplet.op_code = 1;
    bzero(triplet.key, MAX_SIZE);
    
    if(mq_send(q_server, (const char *) &triplet, sizeof(Triplets), 0) == -1){
        perror("mq_send");
        printf("%s", triplet.key);
        mq_close(q_server);
        exit(-1);
    }

    if(mq_receive(q_client, (char *) &res, sizeof(Answers), 0) == -1){
        perror("mq_recv");
        printf("%s", triplet.key);
        mq_close(q_server);
        exit(-1);
    } else{
        printf("%d\n", res.result);
        printf("Init done!\n");
    }

    return 0;
}

int set_value(char* key, char* value1, float value2){
    Triplets triplet;
    Answers answer;
    
    strcpy(triplet.key, key);
    strcpy(triplet.value1, value1); 
    triplet.value2 = value2;
    triplet.op_code = 2;
    strcpy(triplet.q_name, CLIENT_QUEUE);

    if(mq_send(q_server, (const char *) &triplet, sizeof(Triplets), 0) == -1){
        perror("mq_send");
        printf("%s", triplet.key);
        mq_close(q_server);
        exit(-1);
    } else{
        printf("MESSAGE SENT FROM CLIENT TO SERVER (CLIENT SIDE_2)\n");
        printf("Set done!\n");
    }
    mq_receive(q_client, (char *) &answer, sizeof(Answers), 0);

    return 0;
}

int get_value(char* key, char *value1, float *value2){
    Triplets triplet;
    Answers answer;

    strcpy(triplet.key, key);
    triplet.op_code = 3;
    strcpy(triplet.q_name, CLIENT_QUEUE);

    if(mq_send(q_server, (const char *) &triplet, sizeof(Triplets), 0) == -1){
        perror("mq_send");
        exit(-1);
    } else
    {
        printf("MESSAGE SENT FROM CLIENT TO SERVER (CLIENT SIDE_3)\n");
    }

    if(mq_receive(q_client, (char *) &answer, sizeof(Answers), 0) == -1){
        perror("ERROR (CLIENT SIDE_3) -> mq_receive");
        mq_close(q_client);
        exit(-1);
    } else{
        strcpy(value1, answer.value1);
        *value2 = answer.value2;
        printf("RECEIVED KEY %s\n", key);
        printf("RECEIVED VALUE_1 %s\n", value1);
        printf("RECEIVED VALUE_2 %f\n", *value2);
    }

    return 0;
}

int modify_value(char* key, char *value1, float *value2){
    Triplets triplet;
    Answers answer;
      
    strcpy(triplet.key, key);
    strcpy(triplet.value1, value1); 
    triplet.value2 = *value2;
    triplet.op_code = 4;
    strcpy(triplet.q_name, CLIENT_QUEUE);

    if(mq_send(q_server, (const char *) &triplet, sizeof(Triplets), 0) == -1){
        perror("mq_send");
        mq_close(q_server);
        exit(-1);
    } else{
        printf("MESSAGE SENT FROM CLIENT TO SERVER (CLIENT SIDE_3)\n");
        printf("Modify done!\n");
    }
    mq_receive(q_client, (char *) &answer, sizeof(Answers), 0);
      
    return 0;
}

int delete_key(char* key){
    Triplets triplet;
	
    triplet.op_code = 5;
	strcpy(triplet.key, key);
	strcpy(triplet.q_name, CLIENT_QUEUE);

	if(mq_send(q_server, (const char*) &triplet, sizeof(Triplets), 0) == -1){
        perror("mq_send");
        mq_close(q_server);
        exit(-1);
    }else {
        printf("MESSAGE SENT FROM CLIENT TO SERVER (CLIENT SIDE_3)\n");
        printf("Delete done!\n");
    }

    return 0;
}

int exist(char* key){
    Triplets triplet;
    Answers answer;

    strcpy(triplet.key, key);
    strcpy(triplet.q_name, CLIENT_QUEUE);
    triplet.op_code = 6;

    if(mq_send(q_server, (const char *) &triplet, sizeof(Triplets), 0) == -1){
        perror("mq_send");
        mq_close(q_server);
        exit(-1);
    } else
    {
        printf("MESSAGE SENT FROM CLIENT TO SERVER (CLIENT SIDE_3)\n");
    }

    if((mq_receive(q_client, (char *) &answer, sizeof(Answers), 0) == -1)){
        perror("ERROR (CLIENT SIDE_6) -> mq_receive");
        mq_close(q_client);
        exit(-1);
    } else{
        if(answer.result != -1){
            printf("ELEMENT EXISTS\n");
        } else{
            printf("ELEMENT DOESN'T EXIST\n");
        }
    }

    return 0;
}

int num_items(){
    Triplets triplet;
    Answers answer;

    strcpy(triplet.q_name, CLIENT_QUEUE);
    triplet.op_code = 7;

    if(mq_send(q_server, (const char *) &triplet, sizeof(Triplets), 0) == -1){
        perror("mq_send");
        mq_close(q_server);
        exit(-1);
    } else{
        printf("MESSAGE SENT FROM CLIENT TO SERVER (CLIENT SIDE_3)\n");
    }

    if(mq_receive(q_client, (char *) &answer, sizeof(Answers), 0 )== -1){
        perror("ERROR (CLIENT SIDE_3) -> mq_receive");
        mq_close(q_client);
        exit(-1);
    } else{
        printf("NUMBER OF ELEMENTS : %d\n", answer.result);
    }

    return 0;
}

int closeQueues(){
    mq_close (q_client);
    mq_unlink(CLIENT_QUEUE);
    mq_close (q_server);
    mq_unlink(SERVER_QUEUE);
    return 0;
}
