#include "TripletLinkedList.h"
#include <stdlib.h>
#include <string.h>

Node head;
int size;

Node createNode(char* key, char* value1, float value2){
    Node pNode = (Node) malloc(sizeof(LL_Node));
    strcpy(pNode->key, key);
    strcpy(pNode->value1, value1);
    pNode->value2 = value2;
    pNode->nextNode = NULL;
    return pNode;
}

Node getNode(char* key){
    Node res = head;
    if(res->key == key){
        return res;
    } else{
        while(res != NULL){
            if(res->key == key){
                return res;
            }
            if(res->nextNode == NULL){
                return NULL;
            }
            else{
                res = res->nextNode;
            }
        }
    }
    return res;
}

int addNode(char* key, char* value1, float value2){
    if(getNode(key) != NULL){
        return -1;
    }
    Node nNode = createNode(key, value1, value2);
    nNode->nextNode = head;
    head = nNode;
    size++;
    return 0;
}

int deleteNode(char* key){
    Node tmp = getNode(key);
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