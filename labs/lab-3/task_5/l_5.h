#ifndef LAB_5
#define LAB_5

#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PATH 1024
#define NUM_GRADES 5

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

typedef struct student {
	unsigned int id;
	char *name;
	char *surname;
	char *group;
	unsigned char *grades;
} student;

#endif