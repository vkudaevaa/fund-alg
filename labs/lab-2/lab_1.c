#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>
#include <stdarg.h>
#include <time.h>

#define INVALID_ARGUMENTS 1
#define INVALID_OPTION 2
#define MEMORY_ERROR 3
#define STRING_ERROR 4

typedef enum kOpts {
    OPT_L,
    OPT_R,
    OPT_U,
    OPT_N,
    OPT_C
} kOpts;

bool my_atoi(const char *str, int *result) {
    long res = 0;
    int sign = 1;
    int i = 0;

    // Пропуск пробелов
    while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n') {
        i++;
    }

    // Проверка на знак числа
    if (str[i] == '-') {
        sign = -1;
        i++;
    } else if (str[i] == '+') {
        i++;
    }

    // Преобразование строки в число
    while (str[i] >= '0' && str[i] <= '9') {
        res = res * 10 + (str[i] - '0');

        // Проверка переполнения
        if (res > INT_MAX || res < INT_MIN) {
            return false;  
        }

        i++;
    }

    // Проверка на корректность символов
    if (str[i] != '\0') {
        return false;  
    }

    // Возврат результата с учётом знака
    *result = (int)(res * sign);
    return true;
}

int my_strlen(const char* str) {
    int len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

void my_strcpy(char* dest, const char* src) {
    int i = 0;
    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';  
}

void my_strcat(char* dest, const char* src) {
    int dest_len = my_strlen(dest);
    int i = 0;
    while (src[i] != '\0') {
        dest[dest_len + i] = src[i];
        i++;
    }
    dest[dest_len + i] = '\0'; 
}

char* HandlerOptL(char *line, int *error_code) {
    int count = 0;
    while (line[count] != '\0') {
        count++;
    }

    char *str = (char*) malloc(BUFSIZ * sizeof(char));
    if (str == NULL) {
        *error_code = MEMORY_ERROR;
        return NULL;
    }

    snprintf(str, BUFSIZ, "%d", count);  
    return str;
}

char* HandlerOptR(char *line, int *error_code) {
    char *str = (char*) malloc(BUFSIZ * sizeof(char));
    if (str == NULL) {
        *error_code = MEMORY_ERROR;
        return NULL;
    }

    int len = 0;
    my_atoi(HandlerOptL(line, error_code), &len);

    char *p = line + len - 1;
    for (int i = 0; i < len; i++) {
        str[i] = *p--;
    }
    str[len] = '\0'; 
    return str;  
}

char* HandlerOptU(char *line, int *error_code) {
    char *str = (char*) malloc(BUFSIZ * sizeof(char));
    if (str == NULL) {
        *error_code = MEMORY_ERROR;
        return NULL;
    }

    int len = 0;
    my_atoi(HandlerOptL(line, error_code), &len);

    for (int i = 0; i < len; i++) {
        if (i % 2 != 0) {
            str[i] = toupper(line[i]);
        } else {
            str[i] = line[i];
        }
    }
    str[len] = '\0';  
    return str;  
}

char* HandlerOptN(char *line, int *error_code) {
    int len = 0;
    my_atoi(HandlerOptL(line, error_code), &len);
    int digits = 0, letters = 0, others = 0;

    for (int i = 0; i < len; i++) {
        if (isdigit(line[i])) {
            digits++;
        } else if (isalpha(line[i])) {
            letters++;
        } else {
            others++;
        }
    }

    char* arr_digits = (char*)malloc((digits + 1) * sizeof(char));
    char* arr_letters = (char*)malloc((letters + 1) * sizeof(char));
    char* arr_others = (char*)malloc((others + 1) * sizeof(char));

    if (arr_digits == NULL || arr_letters == NULL || arr_others == NULL) {
        *error_code = MEMORY_ERROR;
        free(arr_digits);
        free(arr_letters);
        free(arr_others);
        return NULL;
    }

    int digits_idx = 0, letters_idx = 0, others_idx = 0;

    // Распределение символов
    for (int i = 0; i < len; i++) {
        if (isdigit(line[i])) {
            arr_digits[digits_idx++] = line[i];
        } else if (isalpha(line[i])) {
            arr_letters[letters_idx++] = line[i];
        } else {
            arr_others[others_idx++] = line[i];
        }
    }

    arr_digits[digits_idx] = '\0';
    arr_letters[letters_idx] = '\0';
    arr_others[others_idx] = '\0';

    char* str = (char*)malloc((len + 1) * sizeof(char));
    if (str == NULL) {
        *error_code = MEMORY_ERROR;
        free(arr_digits);
        free(arr_letters);
        free(arr_others);
        return NULL;
    }

    my_strcpy(str, arr_digits);
    my_strcat(str, arr_letters);
    my_strcat(str, arr_others);

    free(arr_digits);
    free(arr_letters);
    free(arr_others);

    return str;  
}

char *HandlerOptC(int argc, char** argv, int seed) { 
    
    int length = 0;
    for (int i = 3; i < argc; i++) { 
        length += my_strlen(argv[i]);
    }
    char *res = malloc(sizeof(char) * (length + 1));
    if (res == NULL) {
        return NULL;
    }
    srand(seed);
    int *used = malloc(sizeof(int) * (argc - 3));
    if (used == NULL) {
        free(res);
        return NULL;
    }
    for (int i = 0; i < argc - 3; i++) {
        used[i] = 0;
    }
    int resindex = 0; 
    for (int i = 0; i < argc - 3; i++) { 
        int rindex = rand() % (argc - 3); 
        while (used[rindex] == 1) {
            rindex = rand() % (argc - 3);
        }
        used[rindex] = 1;
        for (int j = 0; argv[3 + rindex][j] != '\0'; j++) { 
            res[resindex++] = argv[3 + rindex][j];
        }
    }
    res[length] = '\0';
    free(used);
    return res;
}

void GetOpts(int argc, char** argv, kOpts *option, int *error_code){

    if (strcmp(argv[1], "-l") == 0) {
        if (argc != 3){
            *error_code = INVALID_ARGUMENTS;
            return; 
        }
        *option = OPT_L;
    }
    else if (strcmp(argv[1], "-r") == 0) {
        if (argc != 3){
            *error_code = INVALID_ARGUMENTS;
            return; 
        }
        *option = OPT_R;
    }
    else if (strcmp(argv[1], "-u") == 0) {
        if (argc != 3){
            *error_code = INVALID_ARGUMENTS;
            return; 
        }
        *option = OPT_U;
    }
    else if (strcmp(argv[1], "-n") == 0) {
        if (argc != 3){
            *error_code = INVALID_ARGUMENTS;
            return; 
        }
        *option = OPT_N;
    }else if (strcmp(argv[1], "-c") == 0) {
        if (argc < 4){
            *error_code = INVALID_ARGUMENTS;
            return; 
        }
        *option = OPT_C;
    }else{
        *error_code = INVALID_OPTION;
        return;
    }
}

int main(int argc, char** argv) {
    int seed, error_code = 0;
    kOpts opt = OPT_L;
    char* stroka = argv[2];
    char* result = NULL;

    char* (*handlers[4])(char*, int*) = {
        HandlerOptL,
        HandlerOptR,
        HandlerOptU,
        HandlerOptN
    };

    GetOpts(argc, argv, &opt, &error_code);

    if (error_code == INVALID_ARGUMENTS){
        printf("Error: Invalid number of arguments\n");
        return INVALID_ARGUMENTS;
    }else if (error_code == INVALID_OPTION){
        printf("Error: Invalid option\n");
        return INVALID_OPTION;
    }
    
    if (opt == OPT_C){
        my_atoi(argv[2], &seed);
        result = HandlerOptC(argc, argv, seed);
    }else{
        result = handlers[opt](stroka, &error_code);
    }
    
    if (result != NULL) {
        printf("result = %s\n", result); 
        free(result); 
    } else{
        printf("Error: Memory allocation error\n");
        return MEMORY_ERROR;
    }
    return 0;
}
