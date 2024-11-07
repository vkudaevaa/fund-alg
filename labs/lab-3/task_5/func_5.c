#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "l_5.h"


errorCode dataValidation(int argc, char* argv[]){
    if (argc != 3) {return OPT_ERROR_ARGUMENTS;}

    const char *inpath = argv[1];
    char abs_inpath[MAX_PATH];
    const char *outpath = argv[2];
    char abs_outpath[MAX_PATH];

    if (realpath(inpath, abs_inpath) == NULL || realpath(outpath, abs_outpath) == NULL) {
        return OPT_ERROR_FILE;
    }

    if (strcmp(abs_inpath, abs_outpath) == 0){
        return OPT_ERROR_FILE_NAME;
    }
    return OPT_SUCCESS;
}

void freeStudents(student *students, int count) {
    for (int i = 0; i < count; i++) {
        free(students[i].name);
        free(students[i].surname);
        free(students[i].group);
        free(students[i].grades);
    }
    free(students);
}


char* readString(FILE *file) {
	char c = fgetc(file);
	while (isspace(c)) {
		c = fgetc(file);
	}
	ungetc(c, file);
	char *str = NULL;
	int length = 0;
	int capacity = 20;
	str = (char *) malloc(capacity * sizeof(char));
	if (!str) {
		return NULL;
	}
	char ch;
	while (fscanf(file, "%c", &ch) == 1 && ch != ' ' && ch != '\n') {
		if (length + 1 >= capacity) {
			capacity *= 2;
			char *tmp = (char*)realloc(str, capacity * sizeof(char));
			if (!tmp) {
				free(str);
				return NULL;
			}
			str = tmp;
		}
		str[length++] = ch;
	}
	str[length] = '\0';

	return str;
}

int studentsValidation(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isalpha(str[i])) {
            return 0; 
        }
    }
    return 1; 
}

int groupValidation(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isalnum(str[i])) {
            return 0; 
        }
    }
    return 1; 
}

int idValidation(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isdigit(str[i])) {
            return 0; 
        }
    }
    return 1; 
}

double averageScore(student* students){
    double average = 0.0;
    for (int j = 0; j < NUM_GRADES; j++){
        average += students->grades[j];
    }
    average /= NUM_GRADES;
    return average;
}

double overallAverageScore(student* students, int count){
    double total_average = 0.0;
    for (int i = 0; i < count; i++){
        total_average += averageScore(&students[i]);
    }
    total_average /= count;
    return total_average;
}

void foundBestStudent(FILE* file, student* students, int count){
    double average = overallAverageScore(students, count);
    bool flag = true;
    fprintf(file, "Average score = %.2lf\n", average);
    for (int i = 0; i < count; i++){
        if (averageScore(&students[i]) >= average){
            flag = false;
            fprintf(file, "Surname: %s Name: %s Average score: %.2lf\n", students[i].surname, students[i].name, averageScore(&students[i]));
        }
    }
    if (flag){
        fprintf(file, "No students with higher than average scores were found\n");
    }

}

errorCode readingFile(const char* filename, student** students, int* count){
    FILE *input = fopen(filename, "r");
	if (input == NULL){
        fprintf(stderr, "Error: Error open input file\n");
		return OPT_ERROR_FILE;
	}

    *count = 0;
    int capacity = 10;
    *students = (student*)malloc(capacity * sizeof(student));
    if (*students == NULL){
        fclose(input);
        return OPT_ERROR_MEMORY;
    }

    while(true){
        
        if (*count >= capacity){
            capacity *= 2;
            student* tmp = (student*)realloc(*students, capacity * sizeof(student));
            if (tmp == NULL){
                fclose(input);
                freeStudents(*students, *count);
                return OPT_ERROR_MEMORY;
            }
            *students = tmp;
        }

        student* current_student = &(*students)[*count];
        if (fscanf(input, "%u", &current_student->id) != 1) {
			break;
		}

        current_student->name = readString(input);
        current_student->surname = readString(input);
        current_student->group = readString(input);

        if (current_student->name == NULL || current_student->surname == NULL || current_student->group == NULL){
            freeStudents(*students, *count);
            fclose(input);
            return OPT_ERROR_MEMORY;
        }
        if (!studentsValidation(current_student->name) || !studentsValidation(current_student->surname) || !groupValidation(current_student->group)){
            freeStudents(*students, *count);
            fclose(input);
            return OPT_ERROR_FORMAT;

        }
        current_student->grades = (unsigned char*)malloc(NUM_GRADES * sizeof(unsigned char));
        if (current_student->grades == NULL){
            fclose(input);
            freeStudents(*students, *count);
            return OPT_ERROR_MEMORY;
        }
        for (int j = 0; j < NUM_GRADES; j++) {
            fscanf(input, "%hhu", &current_student->grades[j]);
        }
        (*count)++;
    }
    fclose(input);
    return OPT_SUCCESS;
}

void clearStdin() {
	int c;
	while ((c = getchar()) != '\n' && c != EOF);
	//ungetc(c, stdin);
}

void printMainMenu(){
    printf("\nSelect the option:\n");
    printf("<s> - search for all students by ..\n");
    printf("<q> - sorting all students by ..\n");
    printf("<b> - printing the best students ..\n");
    printf("<p> - printing the progress table ..\n");
    printf("<e> - exit\n");
}

int comparatorId(const void* a, const void* b){
    student *st1 = (student *)a;
    student *st2 = (student *)b;
    if (st1->id != st2->id) return (st1->id > st2->id) - (st1->id < st2->id);
}

int comparatorName(const void* a, const void* b){
    student *st1 = (student *)a;
    student *st2 = (student *)b;
    return strcmp(st1->name, st2->name);
}

int comparatorSurname(const void* a, const void* b){
    student *st1 = (student *)a;
    student *st2 = (student *)b;
    return strcmp(st1->surname, st2->surname);
}

int comparatorGroup(const void* a, const void* b){
    student *st1 = (student *)a;
    student *st2 = (student *)b;
    return strcmp(st1->group, st2->group);
}

void printTable(student* students, int count){
    printf("----------------------------------------------------------------------------");
    printf("\n| %-10s | %-15s | %-15s | %-10s | %-10s |\n", "ID", "Name", "Surname", "Group", "Grades");
	printf("----------------------------------------------------------------------------\n");
	for (int i = 0; i < count; i++) {
		printf("| %-10u | %-15s | %-15s | %-10s | %d %d %d %d %d  |\n",
			   students[i].id, students[i].name, students[i].surname,
			   students[i].group, students[i].grades[0], students[i].grades[1], students[i].grades[2], students[i].grades[3], students[i].grades[4]);
        printf("----------------------------------------------------------------------------\n");
	}
	
}