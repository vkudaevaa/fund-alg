#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Функция сравнения элементов массива для сортировки
int compare(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

// Бинарный поиск
int binarySearch(int value, int *arr_b, int size){
    int left = 0;
    int right = size - 1;
    while (left < right) {
        int mid = left + (right - left) / 2;

        if (arr_b[mid] == value) {
            return arr_b[mid]; 
        } else if (arr_b[mid] < value) {
            left = mid + 1; 
        } else {
            right = mid - 1; 
        }
    }
    
    // Выбираем ближайший из двух кандидатов
    if (abs(arr_b[left] - value) < abs(arr_b[right] - value)) {
        return arr_b[left];
    } else {
        return arr_b[right];
    }
}


int main(int argc, char *argv[]){
    // Размер массивов A и B
    int size_a = 10 + rand() % (10000 - 10 + 1);
    int size_b = 10 + rand() % (10000 - 10 + 1);

    int *arr_a = (int *)malloc(size_a * sizeof(int));  
    if (arr_a == NULL) {
        printf("Error: Memory allocation failed\n");
        return 1;
    }
    int *arr_b = (int *)malloc(size_b * sizeof(int));  
    if (arr_b == NULL) {
        printf("Error: Memory allocation failed\n");
        return 2;
    }

    // Заполнение массива A случайными числами и вывод
    for (int i = 0; i < size_a; i++) {
        arr_a[i] = -1000 + rand() % ( 2000 + 1);
    }

    printf("array A: ");
    for (int i = 0; i < size_a; i++) {
        printf("%d ", arr_a[i]);
    }
    printf("\n");

    // Заполнение массива В случайными числами и вывод
    for (int i = 0; i < size_b; i++) {
        arr_b[i] = -1000 + rand() % ( 2000 + 1);
    }

    printf("array B: ");
    for (int i = 0; i < size_b; i++) {
        printf("%d ", arr_b[i]);
    }
    printf("\n");

    int *arr_c = (int *)malloc(size_a * sizeof(int));  
    if (arr_c == NULL) {
        printf("Error: Memory allocation failed\n");
        return 3;
    }

    qsort(arr_b, size_b, sizeof(int), compare);
    
    for (int i = 0; i < size_a; i++){
        int current = binarySearch(arr_a[i], arr_b, size_b);
        arr_c[i] = arr_a[i] + current;
    }

    printf("array C: ");
    for (int i = 0; i < size_a; i++) {
        printf("%d ", arr_c[i]);
    }
    free(arr_a);
    free(arr_b);
    free(arr_c);

    printf("\n");

}
