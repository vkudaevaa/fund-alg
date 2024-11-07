#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SIZE 100

typedef enum errorCode {
    OPT_SUCCESS,
    OPT_ERROR_INVALID_POWER,
    OPT_MEMORY_ERROR
} errorCode;

int add(int a, int b) {
    while (b != 0) {
        int carry = a & b; 
        // Сумма без учёта переноса
        a = a ^ b; 
        b = carry << 1;
    }
    return a;
}

int decrement(int n){
    return add(n, add(~1, 1));
}

void increment(int* n) {
    int mask = 1;
    // Проверяем, есть ли перенос
    while (*n & mask) {
        *n = *n ^ mask; 
        mask <<= 1;   
    }
    *n = *n ^ mask;
}

errorCode convertToBase(int num, int power, char** result){
    int j, index = 0, flag = 0, k = 0;
    char symbols[] = "0123456789ABCDEFGHIJKLMNOPQRSTUV";
    int base = 1 << power;
    // Битовая маска, состоящая из единиц
    int mask = decrement(base);
    if (power < 1 || power > 5){
        return OPT_ERROR_INVALID_POWER;
    }

    char* res = (char*)calloc(SIZE, sizeof(char));
    if (res == NULL){
        return OPT_MEMORY_ERROR;
    }

    if (num == 0){
        res[0] = '0';
        res[1] = '\0';
        increment(&index);
    }else{
        // Получение абсолютного значения
        if (num < 0){
            flag = 1;
            num = ~num;
            increment(&num);
        }

        while (num) {
            // Получение последних power битов числа
            int digit = num & mask;  
            num >>= power;                        
            res[index] = symbols[digit];     
            increment(&index);
        }

        if (flag){
            res[index] = '-';
            increment(&index);
        }
        res[index] = '\0';
    }

    *result = (char*)malloc(index);
    if (*result == NULL){
        free(res);
        return OPT_MEMORY_ERROR;
    }
    
    while (index){
        j = decrement(index);
        (*result)[k] = res[j];
        increment(&k);
        index = decrement(index);

    }
    (*result)[k] = '\0';
    free(res);
    return OPT_SUCCESS;
}


int main(int argc, char* argv[]){
    int power = 1;
    int num = 99;
    char* result = NULL;
    errorCode opt = convertToBase(num, power, &result);
    if (opt == OPT_MEMORY_ERROR){
        printf("Error: Memory allocation error\n");
        return OPT_MEMORY_ERROR;
    }else if (opt == OPT_ERROR_INVALID_POWER){
        printf("Error: Invalid power\n");
    }else if(opt == OPT_SUCCESS){
        printf("%d in %d CC = %s\n", num, 1 << power, result);
    }
    free(result);
}