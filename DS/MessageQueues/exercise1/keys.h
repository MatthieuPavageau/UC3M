#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <string.h>

#define MAX_SIZE 256
#define QUEUE_PERMISSIONS 0660
#define MAX_MESSAGES 10
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

struct mq_attr attributes_s,attributes_c;

int init();
int set_value(char* key, char *value1, float  value2);
int get_value(char* key, char *value1, float *value2);
//int modify_value(char* key, char *value1, float *value2);
int delete_key(char* key);
int num_items();
int exist(char *key);