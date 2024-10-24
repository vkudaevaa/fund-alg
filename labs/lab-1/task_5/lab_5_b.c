#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>


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

// Функция для вычисления факториала
double factorial(int n) {
    if (n == 0) {
        return 1;
    }
    return n * factorial(n - 1);
}

// Определение функции
double f(double x, int n){
    return (pow(-1, n) * pow(x, 2 * n)) / factorial(2 * n);
}

int main(int argc, char *argv[]) {
    if (argc != 3){
        printf("Error: Invalid number of arguments\n");
        return 1;
    }
    // Точность вычислений
    double eps;
    if (!my_atod(argv[1], &eps)) {
        printf("Error: String conversion error\n");
    return 2;
    }

    if (eps <= 0) {
        printf("Error: The accuracy of the calculation must be a positive number\n");
        return 3;
    }
    double x;
    if (!my_atod(argv[2], &x)) {
        printf("Error: String conversion error\n");
    return 2;
    }
    
    int n = 0;
    double sum = 0;
    double prev;
    double cur = f(x, n);
    sum += cur;
    n++;
    // Вычисление суммы бесконечного ряда
    do
    {
        prev = cur;
        cur = f(x, n);
        sum += cur;
        n++;
    } while (fabs(cur - prev) > eps);

    // Вывод результата с указанием вводимой точности
    printf("The value of the sum %.5f with the accuracy of calculations %g\n", sum, eps);

    return 0;
}