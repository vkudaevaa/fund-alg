#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <limits.h>
#include <stdarg.h>
#include <string.h>

typedef enum errorCode {
    OPT_SUCCESS,
    OPT_ERROR_INVALID_ARGUMENTS,
    OPT_INFINITE_NUMBER,
    OPT_ERROR_INVALID_NUMBER,
    OPT_ERROR_INVALID_SYMBOL,
    OPT_MEMORY_ERROR
} errorCode;


errorCode convertIntegerPart(int value, int base, char *result) {
    value = (int)value;
    char res[100];
    int index = 0;
    int sign;

    if (value == 0) {
        result[0] = '0';
        result[1] = '\0';
        return OPT_SUCCESS;
    }
    if (value < 0) {
        sign = -1;
    }
    value = abs(value);

    while (value > 0) {
        int remainder = value % base;
        // Преобразуем остаток в символ (0-9, A-H)
        if (remainder < 10) {
            res[index] = remainder + '0';
        } else {
            res[index] = remainder - 10 + 'A';
        }
        value /= base;
        index++;
    }
    if (sign == -1){
        res[index++] = '-';
    }
    res[index] = '\0';

    // Переворачиваем строку
    for (int i = 0; i < index; i++) {
        result[i] = res[index - 1 - i];
    }
    result[index] = '\0';
    return OPT_SUCCESS;
}

errorCode convertFractionPart(double fraction, int base, char *result) {
    result[0] = '.';
    int index = 1;
    int border = log(INT_MAX) / log(base) + 1;
    int i = 0;


    while (1) {
        fraction *= base;
        int integerPart = (int)fraction;
        if (integerPart < 10) {
            result[index++] = integerPart + '0';
        } else {
            result[index++] = integerPart - 10 + 'A';
        }

        fraction -= integerPart;
        if (fraction == 0) {
            break;
        }
        i++;
        if (i > border){
            return OPT_INFINITE_NUMBER;
        }
    }

    result[index] = '\0';
    return OPT_SUCCESS;
}

errorCode convertToBase(double num, int base, char **result) {
    int integerPart = (int)num;
    double fractionPart = num - integerPart;

    // Переводим целую часть и дробную часть
    char* intResult = (char*)malloc(100 * sizeof(char));
    if (intResult == NULL){
        return OPT_MEMORY_ERROR;
    }
    errorCode optI = convertIntegerPart(integerPart, base, intResult);

    char* fracResult = (char*)malloc(100 * sizeof(char));
    if (fracResult == NULL){
        return OPT_MEMORY_ERROR;
    }
    errorCode optF = convertFractionPart(fractionPart, base, fracResult);
    if (optF == OPT_INFINITE_NUMBER){
        free(intResult);
        free(fracResult);
        return OPT_INFINITE_NUMBER;
    }

    *result = (char*)malloc(strlen(intResult) + strlen(fracResult) + 1);
    if (*result == NULL){
        free(intResult);
        free(fracResult);
        return OPT_MEMORY_ERROR;
    }
    strcpy(*result, intResult);
    strcat(*result, fracResult);

    free(intResult);
    free(fracResult);

    return OPT_SUCCESS;
}


void convertMultiple(int base, int count, ...) {
    va_list args;
    va_start(args, count);

    for (int i = 0; i < count; i++) {
        double num = va_arg(args, double);
        if (num < 0 || num > 1){
            printf("Error: The base does not belong to the interval (0..1)\n");
            continue;
        }
        char* result = NULL;
        errorCode opt = convertToBase(num, base, &result);
        if (opt == OPT_SUCCESS){
            printf("%f in %d CC = %s\n", num, base, result);
            free(result);
        }else if (opt == OPT_INFINITE_NUMBER){
            printf("The number %f has no finite representation in %d CC\n", num, base);
        }else if (opt == OPT_MEMORY_ERROR){
            printf("Error: Memory allocation failed\n");
        }
    }

    va_end(args);
}

int main(int argc, char* argv[]){
    convertMultiple(10, 5, 0.875, 0.125, 0.375, 0.5, 0.04);
}