#ifndef __TRIPLET_LINKED_LIST__
#define __TRIPLET_LINKED_LIST__

struct LL_Node{
    char key[256];
    char value1[256];
    float value2;
    struct LL_Node* nextNode;
};
typedef struct LL_Node* Node;

Node createNode(char* key, char* value1, float value2);
Node getNode(char* key);
int addNode(char* key, char* value1, float value2);
int deleteNode(char* key);
int modifyNode(char* key, char* value1, float value2);
int getSize();
int createLinkedList();
int deleteLinkedList();

#endif