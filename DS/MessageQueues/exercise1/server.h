#include <mqueue.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
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




pthread_mutex_t mutex_triplet;
int triplet_not_copied = TRUE;
pthread_cond_t cond_triplet;
int val;
struct Triplets arr_triplets[MAX_MESSAGES];
struct Triplets arr_triplets_2;


mqd_t q_server;
mqd_t q_client;