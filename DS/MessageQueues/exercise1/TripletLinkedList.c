#include "TripletLinkedList.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Node createNode(char* key, char* value1, float value2){
    Node pNode = (Node) malloc(sizeof(struct LL_Node)); // allocation of a new node
    strcpy(pNode->key, key); // set of the values of the node
    strcpy(pNode->value1, value1);
    pNode->value2 = value2;
    pNode->nextNode = NULL;
    return pNode;
}

Node getNode(char* key){
    Node res = head;
    if(res == NULL){ // the triplet doesn't exist
        return res;
    }else if(strcmp(res->key , key)==0){ // the head corresponds to the triplet we want
        return res;
    } else{
        while(res != NULL){
            if(strcmp(res->key ,key)==0){
                return res;
            }else if(res->nextNode == NULL){
                return NULL;
            }
            else{
                res = res->nextNode;
            }
        }
    }
    return res;
}

int addNode(char* key, char* value1, float value2){ // add a node at the begining of the linked list
    if(getNode(key) != NULL){
        return -1;
    }
    Node nNode = createNode(key, value1, value2);
    if (size > 0)
	{
        nNode->nextNode = head;
    }
    head = nNode;
    printf ("New node : %s - %s - %f\n", head->key, head->value1, head->value2);

	Node tmp = head;
	printf("\nList of the nodes\n");
	while (tmp != NULL)
	{
		printf ("%s - %s - %f\n", tmp->key, tmp->value1, tmp->value2);
		tmp = tmp->nextNode;
	}
    size++;
    return 0;
}

int deleteNode(char* key){
    Node tmp = getNode(key);
    if(tmp == NULL){ // the triplet doesn't exist
        return -1;
    }
    if(tmp == head){
        head = head->nextNode; // if it is the first node, we just take the next node and free the previous head
    } else{
        Node node = head;
        while(node->nextNode != tmp){
           node = node->nextNode;
        }
        node->nextNode = tmp->nextNode;
    }
    free(tmp); // free the space of the node that corresponds to the key parameter
    Node tmp2 = head;
	printf("\nList of the nodes\n");
	while (tmp2 != NULL)
	{
		printf ("%s - %s - %f\n", tmp2->key, tmp2->value1, tmp2->value2);
		tmp2 = tmp2->nextNode;
	}
    size --;
    return 0;
}

int modifyNode(char* key, char* value1, float value2){
    Node tmp = getNode(key);
    if(tmp == NULL){
        return -1;
    }
    strcpy(tmp->value1, value1);
    tmp->value2 = value2;
    return 0;
}

int getSize(){
    return size;
}

int createLinkedList(){ // create a new linked list of triplets
    size = 0;
    head = NULL;
    return 0;
}

int deleteLinkedList(){ // delete the previous linked list
    Node tmp = head;
    while(tmp != NULL){
        free(tmp);
        tmp = tmp->nextNode;
    }
    return 0;
}