#include "keys.h"
#include <stdlib.h>
#include <stdio.h>

int main()
{
    

    int choice;
    printf("CLIENT SERVER PROGRAM IMPLEMENTED USING QUEUES\n");
    printf("OPTION 1 : initialize the system\n"); 
    printf("OPTION 2 : set value\n"); 
    printf("OPTION 3 : get value\n"); 
    printf("OPTION 4 : modify value\n"); 
    printf("OPTION 5 : delete key\n"); 
    printf("OPTION 6 : check if an element exist\n"); 
    printf("OPTION 7 : get the number of elements in a queue\n"); 
    printf("OPTION 8 : exit program\n");
    printf("                  \n");
    printf("Enter your choice:\n"); 
     
     
    
        scanf("%d", &choice);
    switch (choice) { 
    
    case 1: { 
        init();
        break; 
    } 
    case 2: { 
        set_value("key1", "test1", 1.1);
        break; 
    } 
    case 3: { 
        //get_value();
        break; 
    } 
    case 4: { 
        //modify_value();
        break; 
    } 
    case 5: { 
        //delete_key();
        break; 
    } 
    case 6: { 
        //exist();
        break; 
    } 
    case 7: { 
        //num_items();
        break; 
    } 
    default: 
        printf("WRONG INPUT\n"); 
}



        


    return 0; 
}
