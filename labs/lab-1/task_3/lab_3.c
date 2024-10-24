#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>

// Код ошибки для проверки корректности вводимых аргументов
#define ERROR_INVALID_NUMBER_OF_ARGUMENTS -1
#define ERROR_INVALID_OPTION -2
#define INVALID_VALUES -3


#define BUFFER_SIZE 1024

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

bool my_atod(const char *str, double *result) {
    double res = 0.0;
    double fraction = 0.0;
    int sign = 1;
    int i = 0;
    bool is_fraction = false;
    double divisor = 10.0;
    
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

    // Основное преобразование до точки или конца строки
    while ((str[i] >= '0' && str[i] <= '9') || str[i] == '.') {
        if (str[i] == '.') {
            // Ошибка: уже встретили точку
            if (is_fraction) {
                return false;  
            }
            // Начало дробной части
            is_fraction = true;  
        } else {
            if (is_fraction) {
                // Обработка дробной части
                fraction += (str[i] - '0') / divisor;
                divisor *= 10.0;
            } else {
                // Обработка целой части
                res = res * 10 + (str[i] - '0');
            }
        }
        i++;
    }

    // Если после цифр и точки идут любые другие символы —  ошибка
    if (str[i] != '\0') {
        return false;  
    }

    // Возврат результата с учётом знака
    *result = (res + fraction) * sign;
    return true;
}

void HandlerOptQ(char *eps1, char *a1, char *b1, char *c1){
    double a, b, c, eps;
    if (!my_atod(eps1, &eps) || !my_atod(a1, &a) || !my_atod(b1, &b) || !my_atod(c1, &c) ){
        printf("Error: String conversion error\n");
        return;
    }

    if( a == 0 && b != 0){
        double rootl = (-c) / b;
        printf("(%gx) + (%g) = 0              ", b, c);
        printf("One root: x = %.2f\n", rootl);
        return;
    }else if((a == 0 && b == 0 && c == 0) || (a == 0 && b == 0)) {
        printf("(%gx^2) + (%gx) + (%g) = 0     ", a, b, c);
        printf("Warning: Invalid coefficients of the quadratic equation\n");
        return;
    }

    double discriminant = b * b - 4 * a * c;

    // Если дискриминант меньше точности, считаем его нулевым
    if (fabs(discriminant) < eps) {
        discriminant = 0;
    }

    if (discriminant > 0) {
        double root1 = (-b + sqrt(discriminant)) / (2 * a);
        double root2 = (-b - sqrt(discriminant)) / (2 * a);
        printf("(%gx^2) + (%gx) + (%g) = 0     ", a, b, c);
        printf("The roots of the equation: x1 = %.2f, x2 = %.2f\n", root1, root2);
    } else if (discriminant == 0) {
        double root = -b / (2 * a);
        printf("(%gx^2) + (%gx) + (%g) = 0     ", a, b, c);
        printf("One root: x = %.2f\n", root);
    } else {
        printf("(%gx^2) + (%gx) + (%g) = 0     ", a, b, c);
        printf("The equation has no real roots\n");
    }
}

void HandlerOptM(char *a, char *b){
    int a1, b1;
    if (!my_atoi(a, &a1) || !my_atoi(b, &b1)){
        printf("Error: String conversion error\n");
        return;
    }
    if (a1 == 0 || b1 == 0){
        printf("The number must not be 0\n");
        return;
    }
    if (abs(a1) % abs (b1) == 0){
        printf("%d is a multiple of %d\n", a1, b1);
    }else{
        printf("%d is nota multiple of %d\n", a1, b1);
    }
    
}

void HandlerOptT(char *eps1, char *a1, char *b1, char *c1){
    double a, b, c, eps;
    if (!my_atod(eps1, &eps) || !my_atod(a1, &a) || !my_atod(b1, &b) || !my_atod(c1, &c) ){
        printf("Error: String conversion error\n");
        return;
    }
    // Упорядочиваем стороны так, чтобы c было гипотенузой
    if (a > c) {
        double temp = a;
        a = c;
        c = temp;
    }
    if (b > c) {
        double temp = b;
        b = c;
        c = temp;
    }

    // Проверка теоремы Пифагора с учетом точности
    if (fabs(a * a + b * b - c * c) < eps){
        printf("These are the sides of a right triangle\n");
    }else{
        printf("These are not the sides of a right triangle\n");
    }
}

void GetOpts(int argc, char** argv, int *error_code){
    if (strcmp(argv[1], "-q") == 0){
        if (argc != 6){
            *error_code = ERROR_INVALID_NUMBER_OF_ARGUMENTS;
        }else if((strcmp(argv[3], "0") == 0 && strcmp(argv[4], "0") == 0 && strcmp(argv[5], "0") == 0) || (strcmp(argv[3], "0") == 0 && strcmp(argv[4], "0") == 0)){
            *error_code = INVALID_VALUES;
        }else{
        HandlerOptQ(argv[2], argv[3], argv[4], argv[5]);
        HandlerOptQ(argv[2], argv[3], argv[5], argv[4]);
        HandlerOptQ(argv[2], argv[4], argv[3], argv[5]);
        HandlerOptQ(argv[2], argv[4], argv[5], argv[3]);
        HandlerOptQ(argv[2], argv[5], argv[3], argv[4]);
        HandlerOptQ(argv[2], argv[5], argv[4], argv[3]);
        }
    }else if (strcmp(argv[1], "-m") == 0){
        if (argc != 4){
            *error_code = ERROR_INVALID_NUMBER_OF_ARGUMENTS;
        }else{
        HandlerOptM(argv[2], argv[3]);
        }

    }else if (strcmp(argv[1], "-t") == 0){
        if (argc != 6){
            *error_code = ERROR_INVALID_NUMBER_OF_ARGUMENTS;
        }else{
        HandlerOptT(argv[2], argv[3], argv[4], argv[5]);
        }

    }else{
   
        *error_code = ERROR_INVALID_OPTION;    
    }

}


int main(int argc, char *argv[]){
    int error_code = 0;
    GetOpts(argc, argv, &error_code);

    if (error_code == ERROR_INVALID_NUMBER_OF_ARGUMENTS){
        printf("Error: Invalid number of arguments\n");
        return 1;
    }else if (error_code == ERROR_INVALID_OPTION){
        printf("Error: Invalid option\n");
        return 1;
    }else if (error_code == INVALID_VALUES){
        printf("Error: Invalid coefficients of the quadratic equation\n");
        return 1;
    }
}