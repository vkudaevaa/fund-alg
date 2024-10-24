#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

#define PI 3.1415
#define MAX_INTERATION 47

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

bool IsPrime(int n) {
	if (n < 2) {
		return false;
	}
	for (int i = 2; i <= sqrt(n); i++) {
		if (n % i == 0) {
			return false;
		}
	}
	return true;
}

double ProdResult(int t) {
	double product = 1.0;
	for (int p = 2; p <= t; p++) {
		if (IsPrime(p)) {
			product *= (double)(p - 1) / p;
		}
	}
	return product;
}

// Функция для вычисления факториала
long double factorial(int n) {
	long double res = 1.0;
	for (int i = 2; i <= n; i++) {
		res *= i;
	}
	return res;
}

long double BinPow(long double a, int n) {
    long double res = 1;
    while (n != 0) {
        if (n & 1)
            res = res * a;
        a = a * a;
        n >>= 1;
    }
    return res;
}

// Уравнение ln(x)=1
double EquationE(double eps){
    double a = 0.5;
    double b = 1000.5;
    double c;
    c = (a + b) / 2;
    while (fabs(log(c) - 1.0) > eps)
    {
        if (log(c) - 1.0 == 0){
            return c;
        }
        else if ((log(a) - 1.0) * (log(c) - 1.0) < 0){
            b = c;
        }
        else{
            a = c;
        }
        c = (a + b) / 2;
    }
    return c;

}

// Уравнение cos(x)=-1
double EquationPi(double eps){
    double x = 3.0;  
    double cos_value;

    do {
        cos_value = cos(x);  
        x = x - (cos_value + 1) / (-sin(x));  
    } while (fabs(cos_value + 1) > eps);
    return x;
}

// Уравнение e^x=2
double EquationLn(double eps){
    double a = -500;
    double b = 500;
    double c;
    c = (a + b) / 2;
    while (fabs(exp(c) - 2.0) > eps)
    {
        if (exp(c) - 2.0 == 0){
            return c;
        }
        else if ((exp(a) - 2.0) * (exp(c) - 2.0) < 0){
            b = c;
        }
        else{
            a = c;
        }
        c = (a + b) / 2;
    }
    return c;
}

// Уравнение x^2=2
long double EquationSqrt(double epsilon) {
	long double x = 1.0;
	while (fabsl(x * x - 2.0) > epsilon) {
		x -= (x * x - 2.0) / (2 * x);
	}
	return x;
}

// Уравнение для y
long double EquationGamma(double epsilon) {
	double t = 2.0;  
	double prev_x = 0.0;

	for (int i = 0; i < 1000; i++) {
		double product = ProdResult((int)t);

		double current_x = log(t) * product;

		if (fabs(current_x - prev_x) < epsilon) {
			return current_x;
		}

		prev_x = current_x;
		t += 1;
	}

	return prev_x;
}

// Ряд для е
double RowE(double eps){
    int n = 0;
    double sum = 0;
    double prev;
    double cur = 1.0 / factorial(n);
    sum += cur;
    n++;
    do
    {
        prev = cur;
        cur = 1.0 / factorial(n);
        sum += cur;
        n++;
    } while (fabs(cur - prev) > eps || fabs(cur - prev) == 0);
    return sum;
}

// Ряд для pi
double RowPi(double eps){
    int n = 1;
    double sum = 0;
    double prev;
    double cur = pow((-1), n - 1) / (2 * n - 1);
    sum += cur;
    n++;
    do
    {
        prev = cur;
        cur = pow((-1), n - 1) / (2 * n - 1);
        sum  += cur;
        n++;
    } while (fabs(cur - prev) > eps || fabs(cur - prev) == 0);
    return sum;
}

// Ряд для ln(2)
double RowLn(double eps){
    int n = 1;
    double sum = 0;
    double prev;
    double cur = pow((-1), n - 1) / n;
    sum += cur;
    n++;
    do
    {
        prev = cur;
        cur = pow((-1), n - 1) / n;
        sum  += cur;
        n++;
    } while (fabs(cur - prev) > eps || fabs(cur - prev) == 0);
    return sum;
}

// Произведение для sqrt(2)
double RowSqrt(double eps){
    int n = 2;
    double composition = 1;
    double prev;
    double cur = pow(2, pow(2.0, -n));
    composition *= cur;
    n++;
    do
    {
        prev = cur;
        cur = pow(2, pow(2.0, -n));
        composition  *= cur;
        n++;
    } while (fabs(cur - prev) > eps || fabs(cur - prev) == 0);
    return composition;
}

// Ряд для y
long double RowGamma(double epsilon) {
	long double gamma = 0;
	long double term = 0;
	long double gamma_prev;
	int k = 2;

	while (1) {
		gamma_prev = gamma;
		term = (1.0 / pow(floor(sqrt(k)), 2)) - (1.0 / k);
		gamma += term;
		if (fabsl(term) < epsilon && term != 0) {
			break;
		}

		k++;
	}

	return gamma - PI * PI / 6.0;
}

// Предел для е
double LimitE(double eps){
    int n = 1;
    double prev;
    double cur = pow(1.0 + 1.0 / n, n);
    n++;
    do
    {
        prev = cur;
        cur = pow(1.0 + 1.0 / n, n);
        n++;
    } while (fabs(cur - prev) > eps || fabs(cur - prev) == 0);
    return cur;
}

// Предел для pi
double LimitPi(double eps){ 
	double pi = 4;
	double prev_pi = 0.0;
	int n = 1;
	while (fabs(pi - prev_pi) > eps) {
		prev_pi = pi;
		pi *= 4.0 * ((n + 1) * n) / BinPow(2 * n + 1, 2);
		n++;
	}
	return pi;
}

// Предел для ln(2)
double LimitLn(double eps){
    int n = 1;
    double prev;
    double cur = n * (pow(2.0, 1.0 / n) - 1);
    n++;
    do
    {
        prev = cur;
        cur = n * (pow(2.0, 1.0 / n) - 1);
        n++;
    } while (fabs(cur - prev) > eps || fabs(cur - prev) == 0);
    return cur;
}

// Предел для sqrt(2)
double LimitSqrt(double eps){
    double prev;
    double cur = -0.5;
    // Вычисление суммы бесконечного ряда
    do
    {
        prev = cur;
        cur = prev - pow(prev, 2.0) / 2.0 + 1.0;
    } while (fabs(cur - prev) > eps || fabs(cur - prev) == 0);
    return cur;
}

// Предел для y
long double LimitGamma(double epsilon) {
	long double gamma = 0.0;
	long double prev_gamma = 0.0;
	int m = 1;
	long double fact_m = 1;
	while (1) {
		long double sum = 0.0;
		long double fact_k = 1;
		for (int k = 1; k <= m; k++) {
			fact_k *= k;
			long double binom = fact_m / (fact_k * factorial(m - k));
			sum += binom * (k % 2 ? -1.0 : 1.0) * log(fact_k) / k;

		}
		if (m == 1) {
			prev_gamma = -100;
		} else {
			prev_gamma = gamma;
		}
		gamma = sum;
		if (m >= MAX_INTERATION) {
			return gamma;
		}
		if (fabsl(gamma - prev_gamma) < epsilon) {
			break;
		}
		m++;
		fact_m *= m;
	}

	return gamma;
}

int main(int argc, char *argv[]){
    
    if (argc != 2){
        printf("Error: Invalid number of arguments\n");
        return 1;
    }
    double eps; 
    if (!my_atod(argv[1], &eps)){
        printf("Error: String conversion error\n");
        return 2;
    }
    if (eps <= 0) {
        printf("Error: The accuracy of the calculation must be a positive number\n");
        return 3;
    }
    
    
    printf("Equation:\n"); 
    printf("e = %.5f with eps = %g\n", EquationE(eps), eps);
    printf("pi = %.5f with eps = %g\n", EquationPi(eps), eps);
    printf("ln(2) = %.5f with eps = %g\n", EquationLn(eps), eps);
    printf("sqrt(2) = %.5Lf with eps = %g\n", EquationSqrt(eps), eps);
    printf("y = %.5Lf with eps = %g\n", EquationGamma(eps), eps);
    printf("Row:\n");
    printf("e = %.5f with eps = %g\n", RowE(eps), eps);
    printf("pi = %.5f with eps = %g\n", 4 * RowPi(eps), eps);
    printf("ln(2) = %.5f with eps = %g\n", RowLn(eps), eps);
    printf("sqrt(2) = %.5f with eps = %g\n", RowSqrt(eps), eps);
    printf("y = %.5Lf with eps = %g\n", RowGamma(eps), eps);
    printf("Limit:\n");
    printf("e = %.5f with eps = %g\n", LimitE(eps), eps);
    printf("pi = %.5f with eps = %g\n", LimitPi(eps), eps);
    printf("ln(2) = %.5f with eps = %g\n", LimitLn(eps), eps);
    printf("sqrt(2) = %.5f with eps = %g\n", LimitSqrt(eps), eps);
    printf("y = %.5Lf with eps = %g\n", LimitGamma(eps), eps);
    
    return 0;
}