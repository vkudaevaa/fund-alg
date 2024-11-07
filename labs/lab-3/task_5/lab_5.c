#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "l_5.h"
#include "func_5.c"

void menu(student* students, int count, FILE* output){
    while (true){
        printMainMenu();
        printf("The selected option >>> ");
        char command[256];
        char commandtwo[256];
        char commandthree[256];
        char optionthree;
        char optiontwo;
        char option;
        scanf(" %255[^\n]", command);
        sscanf(command, "%c", &option);
        if (option == 's'){
            printf("Select the option to search:\n");
            printf("<i> - search for all students by id\n");
            printf("<n> - search for all students by name\n");
            printf("<s> - search for all students by surname\n");
            printf("<g> - search for all students by group\n");
            clearStdin();
            printf(">>> ");
            scanf(" %255[^\n]", commandtwo);
            sscanf(commandtwo, "%c", &optiontwo);
            switch (optiontwo){
                case 'i': {
                    unsigned int id;
                    printf("Enter the student's ID >>> ");
                    char* in = readString(stdin);
                    if (in == NULL){
                        fprintf(stderr, "Error: Memory allocation error\n");
                        break;
                    }
                    if(!idValidation(in)){
                        printf("Error: Invalid format\n");
                        free(in);
                        continue;
                    }
                    char *end;
                    id = strtoul(in, &end, 10);
                    if (*end != '\0'){
                        printf("Error: Invalid format\n");
                        free(in);
                        break;
                    }
                    student* found_id = NULL;
                    int count_id = 0;
                    bool flag_id = false;
                    for (int i = 0; i < count; i++){
                        if (students[i].id == id){
                            found_id = (student*)realloc(found_id, (count_id + 1) * sizeof(student));
                            if (found_id == NULL){
                                fprintf(stderr, "Error: Memory reallocation error\n");
                                break;
                            }
                            flag_id = true;
                            found_id[count_id] = students[i];
                            double average = averageScore(found_id);
                            fprintf(output, "%u %s %s %.2lf\n", found_id[count_id].id, found_id[count_id].surname, found_id[count_id].group, average);
                            count_id++;
                        }

                    }
                    if (count_id == 0){
                        printf("Student with ID %u not found.\n", id);
                        free(in);
                        break;
                    }
                    if (flag_id){
                        free(found_id);
                        free(in);
                        break;
                    }

                }
                case 'n': {
                    char* name = NULL;
                    printf("Enter the student's name >>> ");
                    name = readString(stdin);
                    if (name == NULL){
                        fprintf(stderr, "Error: Memory reallocation error\n");
                        break;
                    }
                    if (!studentsValidation(name)){
                        printf("Error: Invalid format\n");
                        free(name);
                        continue;
                    }
                    student* found_name = NULL;
                    int count_name = 0;
                    bool flag_name = false;
                    for (int i = 0; i < count; i++){
                        if (strcmp(name, students[i].name) == 0){
                            found_name = (student*)realloc(found_name, (count_name + 1) * sizeof(student));
                            if (found_name == NULL){
                                fprintf(stderr, "Error: Memory reallocation error\n");
                                break;
                            }
                            flag_name = true;
                            found_name[count_name] = students[i];
                            printf("%u %s %s %s\n", found_name[count_name].id, found_name[count_name].name, found_name[count_name].surname, found_name[count_name].group);
                            count_name++;
                        }
                    }
                    if (count_name == 0){
                        printf("Student with name %s not found.\n", name);
                        free(name);
                        break;
                    }
                    if (flag_name){
                        free(found_name);
                        free(name);
                        break;
                    }
                }
                case 's': {
                    char* surname = NULL;
                    printf("Enter the student's surname >>> ");
                    surname = readString(stdin);
                    if (surname == NULL){
                        fprintf(stderr, "Error: Memory reallocation error\n");
                        break;
                    }
                    if (!studentsValidation(surname)){
                        printf("Error: Invalid format\n");
                        free(surname);
                        continue;
                    }
                    student* found_surname = NULL;
                    int count_surname = 0;
                    bool flag_surname = false;
                    for (int i = 0; i < count; i++){
                        if (strcmp(surname, students[i].surname) == 0){
                            found_surname = (student*)realloc(found_surname, (count_surname + 1) * sizeof(student));
                            if (found_surname == NULL){
                                fprintf(stderr, "Error: Memory reallocation error\n");
                                break;
                            }
                            flag_surname = true;
                            found_surname[count_surname] = students[i];
                            printf("%u %s %s %s\n", found_surname[count_surname].id, found_surname[count_surname].name, found_surname[count_surname].surname, found_surname[count_surname].group);
                            count_surname++;
                        }
                    }
                    if (count_surname == 0){
                        printf("Student with surname %s not found.\n", surname);
                        free(surname);
                        break;
                    }
                    if (flag_surname){
                        free(found_surname);
                        free(surname);
                        break;
                    }

                }
                case 'g': {
                    char* group = NULL;
                    printf("Enter the student's group >>> ");
                    group = readString(stdin);
                    if (group == NULL){
                        fprintf(stderr, "Error: Memory reallocation error\n");
                        break;
                    }
                    if (!groupValidation(group)){
                        printf("Error: Invalid format\n");
                        free(group);
                        continue;
                    }
                    student* found_group = NULL;
                    int count_group = 0;
                    bool flag_group = false;
                    for (int i = 0; i < count; i++){
                        if (strcmp(group, students[i].group) == 0){
                            found_group = (student*)realloc(found_group, (count_group + 1) * sizeof(student));
                            if (found_group == NULL){
                                fprintf(stderr, "Error: Memory reallocation error\n");
                                break;
                            }
                            flag_group = true;
                            found_group[count_group] = students[i];
                            printf("%u %s %s %s\n", found_group[count_group].id, found_group[count_group].name, found_group[count_group].surname, found_group[count_group].group);
                            count_group++;
                        }
                    }
                    if (count_group == 0){
                        printf("Student with group %s not found.\n", group);
                        free(group);
                        break;
                    }
                    if (flag_group){
                        free(found_group);
                        free(group);
                        break;
                    }

                }
            }

        }else if(option == 'q'){
            printf("Select the option to sort:\n");
            printf("<i> - sorting all students by id\n");
            printf("<n> - sorting all students by name\n");
            printf("<s> - sorting all students by surname\n");
            printf("<g> - sorting all students by group\n");
            clearStdin();
            printf(">>> ");
            char opn;
            scanf(" %255[^\n]", commandthree);
            sscanf(commandthree, "%c", &optionthree);
            switch (optionthree){
                case 'i': {
                    qsort(students, count, sizeof(student), comparatorId);
                    printf("All students are sorted by ID\n");
                    break;
                }
                case 'n': {
                    qsort(students, count, sizeof(student), comparatorName);
                    printf("All students are sorted by name\n");
                    break;
                }
                case 's': {
                    qsort(students, count, sizeof(student), comparatorSurname);
                    printf("All students are sorted by surname\n");
                    break;
                }
                case 'g': {
                    qsort(students, count, sizeof(student), comparatorGroup);
                    printf("All students are sorted by group\n");
                    break;
                }
            }

        }else if (option == 'p'){
            printTable(students, count);
            continue;

        }else if (option == 'b'){
            foundBestStudent(output, students, count);
            continue;

        }else if (option == 'e'){
            printf("Exiting is completed\n");
            break;
        }else{
            printf("Error: Invalid option\n");
            continue;
        }
    }
}

int main(int argc, char* argv[]){
    errorCode opt = dataValidation(argc, argv);
    if (opt == OPT_ERROR_ARGUMENTS){
        fprintf(stderr, "Error: Invalid number of arguments\n");
        return OPT_ERROR_ARGUMENTS;
    }else if (opt == OPT_ERROR_FILE){
        fprintf(stderr, "Error: Error when using the function realpath\n");
        return OPT_ERROR_FILE;
    }else if (opt == OPT_ERROR_FILE_NAME){
        fprintf(stderr, "Error: The files must have different names\n");
        return OPT_ERROR_FILE_NAME;
    }

    student *students = NULL;
    int count = 0;
    const char* filename = argv[1];
    opt = readingFile(filename, &students, &count);
    if (opt == OPT_ERROR_FILE){
        fprintf(stderr, "Error: Error open input file\n");
        return OPT_ERROR_FILE;
    }else if (opt == OPT_ERROR_MEMORY){
        fprintf(stderr,"Error: Memory allocation error\n");
        return OPT_ERROR_MEMORY;
    }else if (opt == OPT_ERROR_FORMAT){
        fprintf(stderr, "Error: Invalid format\n");
        return OPT_ERROR_FORMAT;
    }

    FILE* output = fopen(argv[2], "w");
    if (output == NULL){
        fprintf(stderr, "Error: Error open output file\n");
        freeStudents(students, count);
        return OPT_ERROR_OPEN_FILE;
    }
    menu(students, count, output);
    freeStudents(students, count);
    fclose(output); 
}