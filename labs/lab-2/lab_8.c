#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>

#define SUCCESS 0
#define ERROR_INVALID_BASE -1
#define ERROR_INVALID_DIGIT -2
#define ERROR_MEMORY_ALLOCATION -3

int char_to_value(char c, int base) {
    if (isdigit(c)) {
        int value = c - '0';
        if (value >= base) return ERROR_INVALID_DIGIT;
        return value;
    }
    
    int value = toupper(c) - 'A' + 10;
    if (value >= base) return ERROR_INVALID_DIGIT;
    
    return value;
}

char value_to_char(int value) {
    if (value < 10) return value + '0';
    return value - 10 + 'A';
}

int sum_digits(char* num1, char* num2, int base, char** result) {
    int len1 = strlen(num1);
    int len2 = strlen(num2);
    int maxLength = len1 > len2 ? len1 : len2;

    *result = (char*)malloc(maxLength + 2); // для переноса и '\0'
    if (*result == NULL) return ERROR_MEMORY_ALLOCATION;

    (*result)[0] = '/';
    (*result)[maxLength + 1] = '\0';

    int carry = 0;
    int i = len1 - 1, j = len2 - 1, k = maxLength;

    while (i >= 0 || j >= 0 || carry) {
        int digit1 = (i >= 0) ? char_to_value(num1[i--], base) : 0;
        int digit2 = (j >= 0) ? char_to_value(num2[j--], base) : 0;

        if (digit1 < 0 || digit2 < 0) return ERROR_INVALID_DIGIT;

        int sum = digit1 + digit2 + carry;
        carry = sum / base;
        (*result)[k--] = value_to_char(sum % base);
    }

    if ((*result)[0] == '/') {
        (*result)[0] = '0';
    }

    // удаление ведущих нулей
    int start = 0;
    while ((*result)[start] == '0' && (*result)[start + 1] != '\0') {
        start++;
    }

    // результат без ведущих нулей
    int new_length = strlen(*result) - start;
    char* trimmed_result = (char*)malloc(new_length + 1);
    if (trimmed_result == NULL){
        free(*result);
        return ERROR_MEMORY_ALLOCATION;
    }
    for (int i = 0; i < new_length; i++) {
        trimmed_result[i] = (*result)[start + i];
    }
    trimmed_result[new_length] = '\0';

    free(*result);
    *result = trimmed_result;
    return SUCCESS;
}

int my_strdup(const char* str, char** copy) {
    *copy = (char*)malloc(strlen(str) + 1);
    if (*copy == NULL) return ERROR_MEMORY_ALLOCATION;
    
    strcpy(*copy, str);
    return SUCCESS;
}

int sum_numbers(int base, int count, char** result, ...) {
    if (base < 2 || base > 36){
        return ERROR_INVALID_BASE;
    }
    va_list args;
    va_start(args, result);

    char* sum = NULL;
    int err = my_strdup("0", &sum);
    if (err != SUCCESS) {
        va_end(args);
        return err;
    }

    for (int i = 0; i < count; i++) {
        char* number = va_arg(args, char*);
        char* temp = NULL;

        err = sum_digits(sum, number, base, &temp);
        if (err != SUCCESS) {
            free(sum);
            va_end(args);
            return err;
        }

        free(sum);
        sum = temp;
    }

    va_end(args);
    *result = sum;

    return SUCCESS;
}

int main() {
    int base = 2;
    char* result = NULL;

    int err = sum_numbers(base, 2, &result, "001", "001");

    if (err == SUCCESS) {
        printf("The sum of the numbers = %s\n", result);
        free(result);
    } else if (err == ERROR_INVALID_BASE) {
        printf("Error: The base does not belong to the interval [2..36]\n");
    } else if (err == ERROR_INVALID_DIGIT) {
        printf("Error: Invalid digit for the given base\n");
    } else if (err == ERROR_MEMORY_ALLOCATION) {
        printf("Error: Memory allocation failed\n");
    }

    return 0;
}
