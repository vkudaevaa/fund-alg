#ifndef LAB_9
#define LAB_9

#include <stdio.h>

#define SIZE 50
#define MAX_PATH 1024

typedef enum errorCode{
	OPT_SUCCESS,
	OPT_ERROR_ARGUMENTS,
    OPT_ERROR_OPTION,
    OPT_ERROR_FILE,
    OPT_ERROR_FILE_NAME,
    OPT_ERROR_OPEN_FILE,
    OPT_ERROR_FORMAT,
	OPT_EOF,
	OPT_ERROR_MEMORY
} errorCode;

typedef struct Node {
    char* word;   
    int count;   
    int length;
    struct Node *left, *right;
} Node;


#endif