#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


// Функция для вычисления минимального основания системы счисления для числа
int find_min_base(const char *num) {
    int max_value = 1;
    int i = 0;
    if (num[i] == '-'){
        i++;
    }
    while (num[i] != '\0') {
        char c = toupper(num[i]);
        if (c >= '0' && c <= '9') {
            if (c - '0' + 1 > max_value) {
                max_value = c - '0' + 1;
            }
        } else if (c >= 'A' && c <= 'Z') {
            if (c - 'A' + 11 > max_value) {
                max_value = c - 'A' + 11;
            }
        }
        i++;
        
    }
    return max_value;
}

// Функция для перевода в 10 СС
int convert_to_decimal(const char *num_str, int base) {
    int result = 0;
    int sign = 1;
    int i = 0;
    int len = strlen(num_str);

    if (num_str[i] == '-') {
        sign = -1;
        i++;
    }
    
    while (i < len) {
        char c = toupper(num_str[i]);
        int value;

        if (c >= '0' && c <= '9') {
            value = c - '0';
        } else if (c >= 'A' && c <= 'Z') {
            value = c - 'A' + 10;
        } 
        // Умножаем предыдущий результат на основание системы счисления и добавляем значение текущей цифры
        result = result * base + value;
        i++;
    }

    result *= sign;
    return result;
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

int main(int argc, char *argv[]){
    if (argc != 3){
        printf("Error: Invalid number of arguments\n");
        printf("Please enter the <input file> <output file>\n");
    }

    FILE *input = fopen(argv[1], "r");
    if (input == NULL) {
        printf("Error: Cannot open input file\n");
        fclose(input);
        return 1;
    }

    FILE *output = fopen(argv[2], "w");
    if (output == NULL) {
        printf("Error: Cannot open output file\n");
        fclose(input);
        fclose(output);
        return 1;
    }

// Буфер для чтения числа
char token[256];  
int base;
int decimal_value;

// Чтение данных из файла
while (fscanf(input, " %255s", token) == 1) {
    char *p = token;
    // Удаление ведущих нулей
    remove_leading_zeros(p);

    // Определение минимального основания
    base = find_min_base(p);

    // Преобразование в десятичную систему
    decimal_value = convert_to_decimal(p, base);

    // Запись в выходной файл
    fprintf(output, "%s %d %d\n", p, base, decimal_value);
}
fclose(input);
fclose(output);
return 0;
}