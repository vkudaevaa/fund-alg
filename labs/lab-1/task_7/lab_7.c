#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


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

void HandlerOptR(char *input_file_1, char *input_file_2, char *output_file){
    FILE *input_1 = fopen(input_file_1, "r");
    if (input_1 == NULL) {
        printf("Error: Cannot open input file\n");
        fclose(input_1);
        return;
    }

    FILE *input_2 = fopen(input_file_2, "r");
    if (input_2 == NULL) {
        printf("Error: Cannot open input file\n");
        fclose(input_2);
        return;
    }

    FILE *output = fopen(output_file, "w");
    if (output == NULL) {
        printf("Error: Cannot open output file\n");
        fclose(input_1);
        fclose(input_2);
        fclose(output);
        return;
    }
    char word1[100], word2[100];
    int file1_end = 0, file2_end = 0;

    while (!file1_end || !file2_end) {
        if (fscanf(input_1, "%99s", word1) == EOF) {
            file1_end = 1; 
        }
        if (fscanf(input_2, "%99s", word2) == EOF) {
            file2_end = 1;
        }

        if (!file1_end) {
            fprintf(output, "%s ", word1);
        }
        if (!file2_end) {
            fprintf(output, "%s ", word2);
        }
    }

    fclose(input_1);
    fclose(input_2);
    fclose(output);
}

void HandlerOptA(char *input_file_1, char *output_file){
    FILE *input = fopen(input_file_1, "r");
    if (input == NULL) {
        printf("Error: Cannot open input file\n");
        fclose(input);
        return;
    }

    FILE *output = fopen(output_file, "w");
    if (output == NULL) {
        printf("Error: Cannot open input file\n");
        fclose(output);
        return;
    }
    
    char word[10];
    int count = 1;
    int value;
    char c;

    while (fscanf(input, "%s", word) != EOF) {
        if (count % 10 == 0) {
            for (int i = 0; i < strlen(word); i++) {
                if (isalpha(word[i]) && isupper(word[i])) {
                    word[i] = tolower(word[i]);
                }
                // Буфер для результата
                char converted_value[32];  
                // Получаем ASCII-код символа
                value = word[i];  
                // Преобразуем в 4 СС
                decimal_to_base(value, converted_value, 4);  
                fprintf(output, "%s ", converted_value);  
            }
        } else if (count % 2 == 0 && count % 10 != 0) {
            for (int i = 0; i < strlen(word); i++) {
                if (isalpha(word[i]) && isupper(word[i])) {
                    word[i] = tolower(word[i]);
                }
            }
            fprintf(output, "%s ", word);
        } else if (count % 5 == 0 && count % 10 != 0) {
            for (int i = 0; i < strlen(word); i++) {
                // Преобразование в ASCII код с основанием 8
                fprintf(output, "%o ", word[i]);
            }
        } else {
            fprintf(output, "%s ", word);
        }
        count++;
    }

    fclose(input);
    fclose(output);
}

char* get_file_name(const char *input_name) {
    // Ищем последний символ '/' в строке
    char *last_slash = strrchr(input_name, '/');
    
    if (last_slash == NULL) {
        // Если '/' не найден, значит путь относительный, возвращаем исходную строку
        return (char*)input_name;
    } else {
        // Возвращаем все, что идет после последнего '/'
        return last_slash + 1;
    }
}

int main(int argc, char *argv[]){
    char *file_1 = argv[2];
    char *file_2 = argv[3];

    if (argc < 4 || argc > 5) {
        fprintf(stderr, "Error: Invalid number of arguments\n");
		return 1;
    } 

    if (strcmp(argv[1], "-r") == 0) {
        if (argc != 5){
            fprintf(stderr, "Error: Invalid number of arguments\n");
		    return 1;
        }
        char *file_3 = argv[4];
        if ((strcmp(get_file_name(argv[2]), get_file_name(argv[4])) == 0) || (strcmp(get_file_name(argv[3]), get_file_name(argv[4])) == 0)){
            fprintf(stderr, "Error: The name of the input and output file must not match\n");
            return 1;
        }
        HandlerOptR(file_1, file_2, file_3);
    }
    else if (strcmp(argv[1], "-a") == 0) {
        if (argc != 4){
            fprintf(stderr, "Error: Invalid number of arguments\n");
		    return 1;
        }
        if (strcmp(get_file_name(argv[2]), get_file_name(argv[3])) == 0){
            fprintf(stderr, "Error: The name of the input and output file must not match\n");
            return 1;
        }
        HandlerOptA(file_1, file_2);
    }
    return 0; 
}