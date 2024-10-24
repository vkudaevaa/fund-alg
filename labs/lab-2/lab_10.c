#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>

#define OPT_MEMORY_ERROR 1

double derivative_point(int degree, int order_derivative, double a, va_list args) {
    double result = 0;
    va_list args_copy;
    va_copy(args_copy, args); 

    for (int i = degree; i >= order_derivative; i--) {
        double coef = va_arg(args_copy, double); 
        double val = coef;
        // Производная порядка order_derivative для степенной функции
        for (int j = 0; j < order_derivative; j++) {
            val *= (i - j);
        }
        // Производная в точке а
        result += val * pow(a, i - order_derivative);
    }
    
    va_end(args_copy); 
    return result;
}

// Формула Тейлора для разложения функции в окрестности точки a
void taylor(int degree, double a, double epsilon, double* result, ...) {
    va_list args;
    va_start(args, result);
    int fact = 1;

    for (int n = 0; n <= degree; n++) {
        double derivative_value = derivative_point(degree, n, a, args);
        if (n > 0){
            fact *= n;
        }
        result[n] = derivative_value / fact;
        if (fabs(result[n]) < epsilon) {
            result[n] = 0;
        }
    }
    va_end(args);
}

int main(int argc, char* argv[]){
    int degree;
    double eps, a;
    double* result = (double*)malloc((degree + 1) * sizeof(double));
    if (result == NULL){
        printf("Error: Memory allocation failed\n");
        return OPT_MEMORY_ERROR;
    }
    printf("Enter the degree of the polynomial: ");
    scanf("%d", &degree);

    printf("Enter the value a: ");
    scanf("%lf", &a);

    printf("Enter epsilon: ");
    scanf("%lf", &eps);

    taylor(degree, a, eps, result, 5.0, -25.0); // f(x) = 2x^2 - 3x + 1
    printf("Resulting coefficients: ");
    for (int i = 0; i < degree + 1; i++){
        printf("%.2lf ", result[i]);
    }
    printf("\n");
    free(result);
}