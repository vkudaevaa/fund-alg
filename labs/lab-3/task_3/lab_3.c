#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "l_3.h"

bool is_alpha(char *s){
    for (size_t i = 0; i < strlen(s); i++) {
        if (!isalpha(s[i])) {
            return false;
        }
    }
    return true;
}

int comparatorOne(const void *a, const void *b) {
    Employee *emp1 = (Employee *)a;
    Employee *emp2 = (Employee *)b;

    if (emp1->salary != emp2->salary) return (emp1->salary > emp2->salary) - (emp1->salary < emp2->salary);
    int last_name_cmp = strcmp(emp1->surname, emp2->surname);
    if (last_name_cmp != 0) return last_name_cmp;
    int first_name_cmp = strcmp(emp1->name, emp2->name);
    if (first_name_cmp != 0) return first_name_cmp;
    return emp1->id - emp2->id;
}

int comparatorTwo(const void *a, const void *b) {
    // Инвертирует результат comparatorOne для сортировки по убыванию 
    return -comparatorOne(a, b);
}

errorCode dataValidation(int argc, char* argv[]){
    if (argc != 4) {return OPT_ERROR_ARGUMENTS;}
    if (argv[2][0] != '/' && argv[2][0] != '-') {return OPT_ERROR_OPTION;}
    if (argv[2][1] != 'a' && argv[2][1] != 'd') {return OPT_ERROR_OPTION;}

    const char *inpath = argv[1];
    char abs_inpath[MAX_PATH];
    const char *outpath = argv[3];
    char abs_outpath[MAX_PATH];

    if (realpath(inpath, abs_inpath) == NULL || realpath(outpath, abs_outpath) == NULL) {
        return OPT_ERROR_FILE;
    }

    if (strcmp(abs_inpath, abs_outpath) == 0){
        return OPT_ERROR_FILE_NAME;
    }
}

errorCode employeesValidation(FILE *file, Employee *emp, size_t *val){
    int result;
    result = fscanf(file, "%u %49s %49s %lf", &emp[*val].id, emp[*val].name, emp[*val].surname, &emp[*val].salary);
    switch (result){
    case 4:
        if (!is_alpha(emp[*val].name) || !is_alpha(emp[*val].surname)){
            return OPT_ERROR_FORMAT;
        }
        return OPT_SUCCESS;
        break;
    case -1:
        return OPT_EOF;
        break;
    
    default:
        return OPT_ERROR_FORMAT;
        break;
    }

}


int main(int argc, char* argv[]){

    errorCode opt = dataValidation(argc, argv);
    if (opt == OPT_ERROR_ARGUMENTS){
        fprintf(stderr, "Error: Invalid number of arguments\n");
        return OPT_ERROR_ARGUMENTS;
    }else if (opt == OPT_ERROR_OPTION){
        fprintf(stderr, "Error: Invalid option\n");
        return OPT_ERROR_OPTION;
    }else if (opt == OPT_ERROR_FILE){
        fprintf(stderr, "Error: Error when using the function realpath\n");
        return OPT_ERROR_FILE;
    }else if (opt == OPT_ERROR_FILE_NAME){
        fprintf(stderr, "Error: The files must have different names\n");
        return OPT_ERROR_FILE_NAME;
    }

    FILE *input = fopen(argv[1], "r");
	if (input == NULL){
        fprintf(stderr, "Error: Error open input file\n");
		return OPT_ERROR_FILE;
	}

    Employee *employees;
    size_t count = 0, capacity = 10;
    employees = (Employee*)malloc(capacity * sizeof(Employee));
    if (employees == NULL){
        fprintf(stderr, "Error: Memory allocation error\n");
        return OPT_ERROR_MEMORY;
    }

    errorCode op;
    while ((op = employeesValidation(input, employees, &count)) == OPT_SUCCESS) {
    
        count++;
        if (count == capacity) {
            capacity *= 2;
            employees = (Employee*)realloc(employees, capacity * sizeof(Employee));
            if (employees == NULL){
                fprintf(stderr, "Error: Memory reallocation error\n");
                return OPT_ERROR_MEMORY;
            }
        }
    }
    fclose(input);

    if (op == OPT_ERROR_FORMAT){
        fprintf(stderr, "Error: Invalid format\n");
        return OPT_ERROR_FORMAT;
    }

    char *flag = argv[2];
    int param = (strcmp(flag, "-a") == 0 || strcmp(flag, "/a") == 0);
    qsort(employees, count, sizeof(Employee), param ? comparatorOne : comparatorTwo);

    FILE *output = fopen(argv[3], "w");
	if (output == NULL){
		fclose(input);
		fclose(output);
        fprintf(stderr, "Error: Error open output file\n");
		return OPT_ERROR_FILE;
	}

    for (size_t i = 0; i < count; i++) {
        fprintf(output, "%u %s %s %.2f\n", employees[i].id, employees[i].name, employees[i].surname, employees[i].salary);
    }
    fclose(output);
    free(employees);    
}

