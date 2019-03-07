#include "server.h"
#include "TripletLinkedList.h"

void process_message(Triplets *triplets){
    Triplets triplet;
    Answers answers;
    
    pthread_mutex_lock(&mutex_triplet);
    memcpy((char *) &triplet, (char *) triplets, sizeof(Triplets));

    triplet_not_copied = FALSE;

    pthread_cond_signal(&cond_triplet);

    pthread_mutex_unlock(&mutex_triplet);

    pthread_mutex_lock(&mutex_switch);
    switch(triplet.op_code){
				case 1:{
						printf("SYSTEM INITIALIZATION ... \n");
						answers.result = 0;
						deleteLinkedList();
						createLinkedList();
						printf("SYSTEM INITIALIZED\n");
						break;
				}

				case 2:{
						if(addNode(triplet.key, triplet.value1, triplet.value2) == -1){
								answers.result = -1;
								printf("The key already exists\n");
						}else{
								answers.result = 0;
								printf("VALUE SET\n");
						}
						break;
				}

				case 3:{
						Node elem = getNode(triplet.key);
						if (elem == NULL){
								printf("ERROR ... \n");
								answers.result = -1;
						}
						else{
								strcpy (answers.value1, elem->value1);
								answers.value2 = elem->value2;
								answers.result = 0;
						}
						break;
				}

				case 4:{
						if(modifyNode(triplet.key, triplet.value1, triplet.value2) == -1){
								answers.result = -1;
						} else{
								answers.result = 0;
						}
						break;
				}

				case 5:{
						if (deleteNode(triplet.key) == -1){
								answers.result = -1;
						} else{
								answers.result = 0;
						}
						break;
				}

				case 6:{
						Node elem = getNode(triplet.key);
						if(elem != NULL){
								answers.result = 0;
						} else{
								answers.result = -1;
						}
						break;
				}

				case 7:{
						answers.result = getSize();
						break;
				}
    }
		pthread_mutex_unlock (&mutex_switch);
		if((q_client = mq_open (triplet.q_name, O_WRONLY)) == -1){
				perror ("Can't open");
		} else{
				if (mq_send (q_client, (const char*) &answers, sizeof(Answers), 0) == -1)
						perror("mq_send");
				mq_close (q_client);
		}
    pthread_exit(NULL);
}

int main(void){
    Triplets triplet;

    struct mq_attr q_attr;
    pthread_attr_t t_attr;
    pthread_t thid;

  	q_attr.mq_maxmsg = 10;
    q_attr.mq_msgsize = sizeof(Triplets);

    if((q_server = mq_open(SERVER_QUEUE, O_CREAT|O_RDONLY, 0777, &q_attr)) == -1){
    		perror("Can't create server queue");
   	}
  
    pthread_mutex_init(&mutex_triplet, NULL);
		pthread_mutex_init(&mutex_switch, NULL);
    pthread_cond_init(&cond_triplet, NULL);
    pthread_attr_init(&t_attr);

    pthread_attr_setdetachstate(&t_attr, PTHREAD_CREATE_DETACHED);

    while(TRUE){
        if(mq_receive(q_server, (char *) &triplet, sizeof(Triplets), 0) == -1){
        	perror("ERROR (SERVER SIDE) -> mq_receive");
        } else{
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
      	while(triplet_not_copied){
            pthread_cond_wait(&cond_triplet, &mutex_triplet);
				}
        triplet_not_copied = TRUE;
        pthread_mutex_unlock(&mutex_triplet);
    }
		return 0;
}