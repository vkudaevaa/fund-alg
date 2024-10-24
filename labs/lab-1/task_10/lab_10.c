#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <stdbool.h>

// Код ошибки для обозначения неправильного символа
#define ERROR_INVALID_CHARACTER -1
#define ERROR_INVALID_BASE -2

// Функция для перевода в 9, 18, 27 и 36 СС
void decimal_to_base(int max_value, char *result, int base) {
    char res[100];
    int index = 0;
    int sign;
    // Если число равно 0, возвращаем строку "0"
    if (max_value == 0) {
        result[0] = '0';
        result[1] = '\0';
        return;
    }
    if (max_value < 0) {
       sign = -1;
    }
    max_value = abs(max_value);

    while (max_value > 0) {
        int remainder = max_value % base;
        // Преобразуем остаток в символ (0-9, A-H)
        if (remainder < 10) {
            res[index] = remainder + '0';  
        } else {
            res[index] = remainder - 10 + 'A'; 
        }
        max_value /= base;
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
    
}

// Функция для удаления незначащих нулей
void remove_leading_zeros(char *num) {
    int i = 0, j = 0;
    int sign = 1;
    if (num[i] == '-') {
        sign = -1;
        i++;
    }

    // Пропускаем все ведущие нули
    while (num[i] == '0') {
        i++;
    }
    // Если все символы были нулями, оставить один ноль
    if (num[i] == '\0') {
        num[0] = '0';
        num[1] = '\0';
        return;
    }
    if (sign == -1){
        num[j] == '-';
        j++;
    }
    // Сдвигаем строку влево, чтобы убрать ведущие нули
    while (num[i] != '\0') {
        num[j++] = num[i++];
    }
    num[j] = '\0';
}

// Функция для проверки совпадения строки
int my_strcmp(const char *str) {
    char stop[] = "Stop";
    int i = 0;
    while (str[i] != '\0' && stop[i] != '\0') {
        // Если символы не совпадают, значит это не "Stop"
        if (str[i] != stop[i]) {
            return 0;  
        }
        i++;
    }
    // Если дошли до конца обеих строк, значит это "Stop"
    return 1;
}

// Функция для перевода в 10 СС
int convert_to_decimal(const char *num_str, int base, int *error_code) {
    int result = 0;
    int sign = 1;
    int i = 0;
    int len = strlen(num_str);

    if (num_str[i] == '-') {
        sign = -1;
        i++;
    }
    
    while (i < len) {
        char c = num_str[i];
        int value;

        if (c >= '0' && c <= '9') {
            value = c - '0';
        } else if (isupper(c)) {
            value = c - 'A' + 10;
        } else {
            *error_code = ERROR_INVALID_CHARACTER;
            return 0;
        }

        if (value >= base) {
            *error_code = ERROR_INVALID_BASE;
            return 0;
        }
        // Умножаем предыдущий результат на основание системы счисления и добавляем значение текущей цифры
        result = result * base + value;
        i++;
    }

    *error_code = 0;
    result *= sign;
    return result;
}

int main(int argc, char *argv[]){
    int base, error_code, decimal_value, len;
    int max_value = 0;
    char input[100], result[100], result_9[100], result_18[100], result_27[100], result_36[100];
    printf("Enter the base of the number system [2..36]: ");
    scanf("%d", &base);

    if (base < 2 || base > 36) {
        printf("The base should be in the range from 2 to 36\n");
        return 1;
    }

    printf("Enter the numbers in the selected number system (to complete, enter 'Stop')\n");
    while (1) {
        scanf("%s", input);
        if (my_strcmp(input)) { 
            break;
        }
        decimal_value = convert_to_decimal(input, base, &error_code);

        if (error_code == ERROR_INVALID_CHARACTER) {
            printf("Error: Invalid character in the number\n"); 
        } else if (error_code == ERROR_INVALID_BASE) {
            printf("Error: Invalid character for this number system\n");
        }
        if (abs(decimal_value) > abs(max_value)){
            max_value = decimal_value;
            if (!my_strcmp(input)){
                len = strlen(input);
                for (int i = 0; i < len; i++){
                    result[i] = input[i];

                }
                result[len] = '\0';
            }
        }
        
    }
    remove_leading_zeros(result);
    printf("The maximum number modulo: %s\n", result);
    decimal_to_base(max_value, result_9, 9);
    printf("In 9 CC: %s\n", result_9);
    decimal_to_base(max_value, result_18, 18);
    printf("In 18 CC: %s\n", result_18);
    decimal_to_base(max_value, result_27, 27);
    printf("In 27 CC: %s\n", result_27);
    decimal_to_base(max_value, result_36, 36);
    printf("In 36 CC: %s\n", result_36);

    return 0;
}