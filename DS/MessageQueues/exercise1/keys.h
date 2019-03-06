#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <string.h>



int init();
int set_value(char* key, char *value1, float  value2);
int get_value(char* key, char *value1, float *value2);