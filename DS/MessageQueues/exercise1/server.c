#include "server.h"

void process_message(Triplets *triplets){
	Triplets triplet;

	int result;
	int length;
	int count;
	int exists;

	struct mq_attr q_attr;

	q_attr.mq_maxmsg = MAX_MESSAGES;
	q_attr.mq_msgsize = sizeof(Triplets);

	pthread_mutex_lock(&mutex_triplet);
	memcpy((char *) &triplet, (char *) triplets, sizeof(Triplets));

	triplet_not_copied = FALSE;

	pthread_cond_signal(&cond_triplet);

	pthread_mutex_unlock(&mutex_triplet);

	if((q_client = mq_open(triplet.q_name, O_RDWR,0777,&q_attr))==-1)
	{
		perror("Can't open client queue");
		pthread_exit(NULL);
	}
	switch(triplet.op_code)
	{
		case 1:

		printf("SYSTEM INITIALIZATION ... \n");
		result = 1;
		if((mq_send(q_client, (char *) &result, sizeof(int), 0)==-1))
		{
			perror("mq_send\n");
			mq_close(q_client);
		} else {
			printf("SYSTEM INITIALIZED\n");
		}
		mq_close(q_client);
		break;

		case 2:

		for (int i = 0 ; i<MAX_MESSAGES ; ++i)
		{
			length = strlen(arr_triplets[i].key);
			if (length == 0)
			{
				strcpy(arr_triplets[i].key , triplet.key);
				strcpy(arr_triplets[i].value1 , triplet.value1);
				arr_triplets[i].value2 = triplet.value2; 
				strcpy(arr_triplets[i].q_name, triplet.q_name);
				arr_triplets[i].op_code = triplet.op_code; 
				break;
			}
		}


		for ( int j = 0; j < MAX_MESSAGES ; j++)
		{
			printf("%s %s %f\n",arr_triplets[j].key, arr_triplets[j].value1, arr_triplets[j].value2 );
		}

		printf("VALUE SET");

		break;

		case 3:

		for ( int i = 0; i < MAX_MESSAGES; ++i)
		{
			
			if (strcmp(arr_triplets[i].key,triplet.key))
			{
				strcpy(triplet.key, arr_triplets[i].key);
				strcpy(triplet.value1, arr_triplets[i].value1);
				
				break;
			}
		}



		if((mq_send(q_client, (const char *) &triplet, sizeof(Triplets), 0)==-1))
		{
			perror("mq_send\n");
			mq_close(q_client);
		} else {
			printf("VALUE SENT FROM SERVER TO CLIENT\n");
		}



		mq_close(q_client);
		mq_close(q_server);
		break;

		case 4:

		break;

		case 5:

		// for ( int i=0; i < MAX_MESSAGES ; ++i)
		// {

		// 	if (strcmp(arr_triplets[i].key,triplet.key) !=0)
		// 	{
		// 		arr_triplets[i].key     = NULL;
		// 		arr_triplets[i].value1  = NULL;
		// 		arr_triplets[i].value2  = NULL;
		// 		arr_triplets[i].op_code = NULL;
		// 		arr_triplets[i].q_name  = NULL;
		// 		break;
		// 	}

		// }

		// for ( int j = 0; j < MAX_MESSAGES ; j++)
		// {
		// 	printf("%s %s %f\n",arr_triplets[j].key, arr_triplets[j].value1, arr_triplets[j].value2 );
		// }


		break;

		case 6:

		for ( int i=0; i < MAX_MESSAGES ; ++i)
		{

			if (strcmp(arr_triplets[i].key,triplet.key) !=0)
			{
				exists = 1;
				break;
			} else
			{
				exists = 0;
			}

		}
		if((mq_send(q_client, (const char *) &exists, sizeof(int), 0)==-1))
		{
			perror("mq_send\n");
			mq_close(q_client);
		} else {
			printf("%d\n", exists );
			printf("VALUE SENT FROM SERVER TO CLIENT\n");
		}
		mq_close(q_client);

		break;

		case 7:
		count = 0;
		for ( int i=0; i < MAX_MESSAGES ; ++i)
		{
			length = strlen(arr_triplets[i].key);
			if (length != 0)
			{
				count=count+1;
			}

		}

		if((mq_send(q_client, (const char *) &count, sizeof(int), 0)==-1))
		{
			perror("mq_send\n");
			mq_close(q_client);
		} else {
			printf("VALUE SENT FROM SERVER TO CLIENT\n");
		}
		mq_close(q_client);
		break;
		case 8:
		// mq_unlink(SERVER_QUEUE);
		// mq_unlink(CLIENT_QUEUE);
		break;
	}



	mq_close(q_client);
	mq_close(q_server);


	pthread_exit(NULL);
}

int main(void)
{


	mqd_t q_server;

	Triplets triplet;

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

	pthread_mutex_init(&mutex_triplet, NULL);
	pthread_cond_init(&cond_triplet, NULL);
	pthread_attr_init(&t_attr);

	pthread_attr_setdetachstate(&t_attr, PTHREAD_CREATE_DETACHED);

	while(TRUE)
	{
		if((mq_receive(q_server, (char *) &triplet, sizeof(Triplets), 0)==-1))
		{
			perror("ERROR (SERVER SIDE) -> mq_receive");
		} else
		{
			printf("------------------------------------------------------\n");
			printf("KEY RECEIVED AT SERVER SIDE            : %s\n",triplet.key);
			printf("VALUE 1 RECEIVED AT SERVER SIDE        : %s\n",triplet.value1);
			printf("VALUE 2 RECEIVED AT SERVER SIDE        : %f\n",triplet.value2);
			printf("QUEUE NAME RECEIVED AT SERVER SIDE     : %s\n",triplet.q_name);
			printf("OPERATION CODE RECEIVED AT SERVER SIDE : %d\n",triplet.op_code);
			printf("------------------------------------------------------\n");
		}


		pthread_create(&thid, &t_attr, (void (*))process_message, &triplet);
		pthread_mutex_lock(&mutex_triplet);
		while(triplet_not_copied)
			pthread_cond_wait(&cond_triplet, &mutex_triplet);
		triplet_not_copied = TRUE;
		pthread_mutex_unlock(&mutex_triplet);
	}




	mq_close(q_server);
}