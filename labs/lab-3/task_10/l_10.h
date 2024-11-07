#ifndef LAB_10
#define LAB_10

#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PATH 1024

typedef enum errorCode{
	OPT_SUCCESS,
	OPT_ERROR_ARGUMENTS,
    OPT_ERROR_FILE,
    OPT_ERROR_FILE_NAME,
    OPT_ERROR_OPEN_FILE,
    OPT_ERROR_FORMAT,
	OPT_ERROR_MEMORY
} errorCode;

typedef struct Node {
    char data;
    struct Node** children;  
    int child_count; 
    int child_capacity;       
} Node;

#endif