#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 1024

typedef enum kOpts {
    OPT_D,
    OPT_I,
    OPT_S,
    OPT_A
} kOpts;

int GetOpts(int argc, char** argv, kOpts* option) {
    if (argc < 3 || argc > 4) {
        return -1;
    }

    if (strcmp(argv[1], "-d") == 0 && argc == 3) {
        *option = OPT_D;
        return 0;
    }
    else if (strcmp(argv[1], "-nd") == 0 && argc == 4) {
        *option = OPT_D;
        return 1;
    }
    else if (strcmp(argv[1], "-i") == 0 && argc == 3) {
        *option = OPT_I;
        return 0;
    }
    else if (strcmp(argv[1], "-ni") == 0 && argc == 4) {
        *option = OPT_I;
        return 1;
    }
    else if (strcmp(argv[1], "-s") == 0 && argc == 3) {
        *option = OPT_S;
        return 0;
    }
    else if (strcmp(argv[1], "-ns") == 0 && argc == 4) {
        *option = OPT_S;
        return 1;
    }
    else if (strcmp(argv[1], "-a") == 0 && argc == 3) {
        *option = OPT_A;
        return 0;
    }
    else if (strcmp(argv[1], "-na") == 0 && argc == 4) {
        *option = OPT_A;
        return 1;
    }

    return -1;

}

void HandlerOptD(FILE *input, FILE *output){
    char ch;
    while ((ch = fgetc(input)) != EOF) {
        // Если символ не цифра, записываем его в выходной файл
        if (!isdigit(ch)) {
            fputc(ch, output);
        }
    }

}

void HandlerOptI(FILE *input, FILE *output){
    char buffer[BUFFER_SIZE];
    int count;
    while (fgets(buffer, BUFFER_SIZE, input) != NULL) {
        count = 0;

        // Проходим по строке и считаем латинские буквы
        for (int i = 0; buffer[i] != '\0'; i++) {
            if (isalpha(buffer[i])) {
                count++;
            }
        }

        // Записываем результат в выходной файл
        fprintf(output, "The number of letters in a line: %d\n", count);
    }
    
}

void HandlerOptS(FILE *input, FILE *output){
    char buffer[BUFFER_SIZE];
    int count;
    while (fgets(buffer, BUFFER_SIZE, input) != NULL) {
        count = 0;

        // Проходим по строке и считаем знаки
        for (int i = 0; buffer[i] != '\0'; i++) {
            if (!isalpha(buffer[i]) && !isdigit(buffer[i]) && !isspace(buffer[i])) {
                count++;
            }
        }

        // Записываем результат в выходной файл
        fprintf(output, "The number of characters in a line: %d\n", count);
    }
    
}

void HandlerOptA(FILE *input, FILE *output){
    char buffer[BUFFER_SIZE];
	while (fgets(buffer, BUFFER_SIZE, input) != NULL) {
		for (int i = 0; buffer[i] != '\0'; i++) {
			if (isdigit(buffer[i])) {
				fputc(buffer[i], output);
			} else {
				fprintf(output, "%x", buffer[i]);
			}
		}
	}
    
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
    kOpts opt = OPT_D;
    const char *input_file = argv[2];
    char *output_file = NULL;


    void (*handlers[4])(FILE*, FILE*) = {HandlerOptD, HandlerOptI, HandlerOptS, HandlerOptA};
    int result = GetOpts(argc, argv, &opt);

    if (result == -1){
        fprintf(stderr, "Error: Invalid number of arguments\n");
		return 1;
    }else if(result == 0){
        const char *input_abs_file = get_file_name(input_file);
	    output_file = (char*)malloc(strlen("out_") + strlen(input_abs_file + 1)); // +1 для '\0'
	    if (output_file == NULL) {
		    fprintf(stderr, "Error: Memory allocation error\n");
		    free(output_file);
            return 1;
	    }
        strcpy(output_file, "out_"); 
        strcat(output_file, input_abs_file);
    }else if(result == 1){
        if (strcmp(get_file_name(argv[3]), get_file_name(argv[2])) == 0){
            fprintf(stderr, "Error: The name of the input and output file must not match\n");
            return 1;
        }else{
            output_file = argv[3];
        }
    }

    FILE *input = fopen(input_file, "r");
    if (input == NULL) {
        printf("Error: Cannot open input file\n");
        fclose(input);
        return 1;
    }

    FILE *output = fopen(output_file, "w");
    if (output == NULL) {
        printf("Error: Cannot open output file\n");
        fclose(input);
        fclose(output);
        return 1;
    }

    handlers[opt](input, output);
	fclose(input);
	fclose(output);
    if (result == 0){
        free(output_file);
    }
    return 0;
   


}