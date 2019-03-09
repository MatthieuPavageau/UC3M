#include "keys.h"

int main(){

    int choice;
    int test = 0;
    while(test == 0){
        printf("CLIENT SERVER PROGRAM IMPLEMENTED USING QUEUES\n");
        printf("OPTION 1 : initialize the system\n"); 
        printf("OPTION 2 : set value\n"); 
        printf("OPTION 3 : get value\n"); 
        printf("OPTION 4 : modify value\n"); 
        printf("OPTION 5 : delete key\n"); 
        printf("OPTION 6 : check if an element exist\n"); 
        printf("OPTION 7 : get the number of elements\n"); 
        printf("OPTION 8 : exit program\n");
        printf("                  \n");
        printf("Enter your choice:\n"); 
        
        scanf("%d", &choice);

        switch(choice){ 
            case 1: { 
                printf("You've chosen option 1\n");
                init();
                break; 
            } 

            case 2: { 
                printf("You've chosen option 2\n");
                int n;
                char k[100],b[100];
                float z;
                printf("Enter how many triplets you want to send to server : \n");
                scanf("%d", &n);
                for (int i = 0; i < n ; ++i){
                    printf("Enter key :\n");
                    scanf("%99s", k);
                    printf("Enter value 1 :\n");
                    scanf("%99s", b);
                    printf("Enter value 2 :\n");
                    scanf("%f", &z);
                    set_value(k, b, z);
                }
                break; 
            } 

            case 3: { 
                char value1[256];
                float value2;
                char k[100];
                printf("Enter the key of the triplet you want to get : \n");
                scanf("%99s", k);
                get_value(k, value1, &value2);
                break; 
            } 

            case 4: {
                char k[100],b[100];
                float z;
                printf("Enter key of the triplet you want to modify:\n");
                scanf("%99s", k);
                printf("Enter new value 1 :\n");
                scanf("%99s", b);
                printf("Enter new value 2 :\n");
                scanf("%f", &z);
                modify_value(k, b, &z);
                break; 
            } 

            case 5: {
                char k[100];
                printf("Enter key of the triplet you want to delete:\n");
                scanf("%99s", k);
                delete_key(k);
                break; 
            } 

            case 6: { 
                char k[100];
                printf("Enter key of the triplet you want to check if it exists:\n");
                scanf("%99s", k);
                exist(k);
                break;
            } 

            case 7: { 
                num_items();
                break; 
            } 

            case 8:{
                test = 1;
                break;
            }

            default:{
                printf("WRONG INPUT\n");
            }
        }
    }

    closeQueues();

    return 0; 
}