#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

// Функция для преобразования строки в int
bool my_atoi(const char *str, int *result) {
    long res = 0;
    int sign = 1;
    int i = 0;

    // Пропуск пробелов
    while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n') {
        i++;
    }

    // Проверка на знак числа
    if (str[i] == '-') {
        sign = -1;
        i++;
    } else if (str[i] == '+') {
        i++;
    }

    // Преобразование строки в число
    while (str[i] >= '0' && str[i] <= '9') {
        res = res * 10 + (str[i] - '0');

        // Проверка переполнения
        if (res > INT_MAX || res < INT_MIN) {
            return false;  
        }

        i++;
    }

    // Проверка на корректность символов
    if (str[i] != '\0') {
        return false;  
    }

    // Возврат результата с учётом знака
    *result = (int)(res * sign);
    return true;
}

// Функция для обмена двух элементов
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Поиск и обмен минимального и максимального элементов
void SwapMinMax(int arr[], int size) {
    int minIndex = 0, maxIndex = 0;

    // Проход по массиву для поиска минимального и максимального элементов
    for (int i = 1; i < size; i++) {
        if (arr[i] < arr[minIndex]) {
            minIndex = i;
        }
        if (arr[i] > arr[maxIndex]) {
            maxIndex = i;
        }
    }

    // Обмен местами минимального и максимального элементов
    swap(&arr[minIndex], &arr[maxIndex]);
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Error: Invalid number of arguments\n");
        return 1;
    }

    int size;
    int a;
    int b;

    if (!my_atoi(argv[1], &size)) {
        printf("Error: String conversion error\n");
        return 2;
    }
    if (!my_atoi(argv[2], &a)) {
        printf("Error: String conversion error\n");
        return 2;
    }
    if (!my_atoi(argv[3], &b)) {
        printf("Error: String conversion error\n");
        return 2;
    }

    int arr[size];
    for (int i = 0; i < size; i++) {
        arr[i] = a + rand() % (b - a + 1);
    }

    // Вывод исходного массива
    printf("The original array: ");
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    SwapMinMax(arr, size);

    // Вывод массива после обмена
    printf("Result: ");
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    return 0;
}
