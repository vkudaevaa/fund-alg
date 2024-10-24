#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#define TAB_SIZE 4

typedef enum errorCode {
    OPT_SUCCESS,
    OPT_MEMORY_ERROR
} errorCode;

void print(const char** file_path, int* first_line, int* first_index){
    printf("file %s line %d position %d\n", *file_path, *first_line, *first_index);
}

errorCode find_substring_in_file(const char* substring, const char* file_path) {
    FILE *file = fopen(file_path, "r");
    if (file == NULL){
        return OPT_MEMORY_ERROR;
    }
    int line = 1, position = 1, index = 0, first_index = 0, first_line = 1;
    char c;
    while ((c = fgetc(file)) != EOF){
        if (c == substring[index]){
            index++;
            if (index == 1){
                first_index = position;
                first_line = line;
            }
            
            if (substring[index] == '\0'){
                print(&file_path, &first_line, &first_index);
                index = 0;
                first_index = 0;
            }
        }else{
            index = 0;
            first_index = 0;
        }
        if (c == '\n'){
            line++;
            position = 1;
        }else if (c == '\t'){
            position += TAB_SIZE;
        }else{
            position++;
        }

    }
    fclose(file);
    return OPT_SUCCESS;
}    

errorCode search_in_files(const char* substring, int num_files, ...){
    va_list args;
    va_start(args, num_files);
    for (int i = 0; i < num_files; i++) {
        const char* file_path = va_arg(args, const char*);
        errorCode opt = find_substring_in_file(substring, file_path);
        if (opt == OPT_MEMORY_ERROR){
            return OPT_MEMORY_ERROR;
        }
    }

    va_end(args);

}

int main(int argc, char* argv[]){
    const char* substring = "\t";
    errorCode opts = search_in_files(substring, 2, "file1.txt", "file2.txt");
    if (opts == OPT_MEMORY_ERROR){
        printf("Error: Memory allocation error\n");
        return OPT_MEMORY_ERROR;
    }
    return OPT_SUCCESS;
}