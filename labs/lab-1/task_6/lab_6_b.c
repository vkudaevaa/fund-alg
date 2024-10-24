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



// Определение функции f(x) = exp(pow(x, 2) / (-2)
double f(double x) {
    return exp(pow(x, 2) / (-2));
}

// Функция для вычисления интеграла методом средних прямоугольников
double middle_rectangles(double a, double b, int n) {
    double h = (b - a) / n;  // Шаг разбиения
    double sum = 0.0;
    double x_i;
    double x_i_1;
    double x_mid;

    // Цикл по всем отрезкам
    for (int i = 1; i <= n; i++) {
       
        // Вычисление средней точки на очередном отрезке
        if (i == 1){
            x_i = a;
            x_i_1 = a + h;
        }
        x_mid = (x_i + x_i_1) / 2;
        sum += f(x_mid);
        x_i = x_i_1;
        x_i_1 = x_i + h;
    }
    // Приближённое значение площади под графиком функции
    return sum * h;  
}

// Метод Рунге для оценки погрешности интегрирования
double Runge_method(double I_1, double eps, int n, double a, double b){
    double I_2 = I_1;
    do
    {   
        I_1 = I_2;
        n *= 2;
        I_2 = middle_rectangles(a, b, n);
    } while (fabs(I_1 - I_2) / 3 > eps);

    return I_2;
}

int main(int argc, char *argv[]) {
    if (argc != 2){
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
    // Нижний предел интегрирования
    double a = 0.0; 
    // Верхний предел интегрирования  
    double b = 1.0; 
    // Количество отрезков  
    int n = 2;     

    // Вычисление интеграла
    double first = middle_rectangles(a, b, n);
    double result = Runge_method(first, eps, n, a, b);
    
    // Вывод результата с указанием вводимой точности
    printf("The value of the integral %.5f with the accuracy of calculations %g\n", result, eps);

    return 0;
}
