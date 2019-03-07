#include "TripletLinkedList.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Node head;
int size;

Node createNode(char* key, char* value1, float value2){
    Node pNode = (Node) malloc(sizeof(struct LL_Node));
    strcpy(pNode->key, key);
    strcpy(pNode->value1, value1);
    pNode->value2 = value2;
    pNode->nextNode = NULL;
    return pNode;
}

Node getNode(char* key){
    Node res = head;
    if(res == NULL){
        return res;
    }else if(strcmp(res->key , key)==0){
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
    Node tmp = head;
	printf("List:\n");
	while(tmp != NULL){
		printf ("%s %s %f\n", tmp->key, tmp->value1, tmp->value2);
		tmp = tmp->nextNode;
	}
    return res;
}

int addNode(char* key, char* value1, float value2){
    if(getNode(key) != NULL){
        return -1;
    }
    Node nNode = createNode(key, value1, value2);
    if (size > 0)
	{
        nNode->nextNode = head;
    }
    head = nNode;
    size++;
    return 0;
}

int deleteNode(char* key){
    Node tmp = getNode(key);
    if(tmp == NULL){
        return -1;
    }
    if(tmp == head){
        head = head->nextNode;
    } else{
        Node node = head;
        while(node->nextNode != tmp){
           node = node->nextNode;
        }
        node->nextNode = tmp->nextNode;
    }
    free(tmp);
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

int createLinkedList(){
    size = 0;
    head = NULL;
    return 0;
}

int deleteLinkedList(){
    Node tmp = head;
    while(tmp != NULL){
        free(tmp);
        tmp = tmp->nextNode;
    }
    return 0;
}