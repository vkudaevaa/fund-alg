#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#define INVALID_ARGUMENTS 1
#define STRING_ERROR 2
#define INVALID_EPSILON 3

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

double f1(double x) {
    return exp(x) - 2.0;
}

double f2(double x) {
    return 1.0 / x - x;
}

double f3(double x) {
    return pow(x, 3) - x - 1;
}

double dichotomy(double a, double b, double eps, double (*f) (double)){
    
    double c;
    c = (a + b) / 2.0;
    while (fabs(f(c)) > eps)
    {
    
        if (f(a) * f(c) < 0){
            b = c;
        }
        else{
            a = c;
        }
        c = (a + b) / 2.0;
    }
    return c;
}
int CheckBoundaries(double a, double b, double (*f)(double)) {
    return f(a) * f(b) >= 0 ? 1 : 0;
}

int main(int argc, char *argv[]){
    if (argc != 4){
        printf("Error: Invalid number of arguments\n");
        return INVALID_ARGUMENTS;
    }
    double a;
    if (!my_atod(argv[1], &a)){
        printf("Error: String conversion error\n");
        return STRING_ERROR;
    }
    double b;
    if (!my_atod(argv[2], &b)){
        printf("Error: String conversion error\n");
        return STRING_ERROR;
    }
    double eps; 
    if (!my_atod(argv[3], &eps)){
        printf("Error: String conversion error\n");
        return STRING_ERROR;
    }
    if (eps <= 0) {
        printf("Error: The accuracy of the calculation must be a positive number\n");
        return INVALID_EPSILON;
    }

    // Массив указателей на функции
    double (*functions[])(double) = {f1, f2, f3};
    const char *equations[] = {
        "e^x - 2 = 0",
        "1 / x - x = 0",
        "x^3 - x - 1 = 0"
    };

    // Обработка всех функций через цикл
    for (int i = 0; i < 3; ++i) {
        if (CheckBoundaries(a, b, functions[i])) {
            printf("Error: Invalid interval boundaries for equation %s\n", equations[i]);
            continue;
        }

        double root = dichotomy(a, b, eps, functions[i]);
        printf("%s\nx = %.5f with accuracy %g\n", equations[i], root, eps);
    }

    return 0;
}