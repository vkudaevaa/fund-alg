#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <float.h>
#include <ctype.h>
#include <stdbool.h>
#include <limits.h>

typedef enum errorCode { 
    OPT_SUCCESS, 
    OPT_FAIL, 
    OPT_ERROR_OVERFLOW, 
    OPT_ERROR_INVALID, 
    OPT_ERROR_NAN 
} errorCode;

errorCode geometricMean(double *res, int count, ...){
	if (count < 1) {
		return OPT_ERROR_INVALID;
	}
    double val = 1.0;
    va_list args;           
    va_start(args, count); 
    for (int i = 0; i < count; i++) {
        double num = va_arg(args, double);  
        val *= num;
    }
    va_end(args);
    *res = pow(val, 1.0 / count);
    if (isinf(*res)) {
	    return OPT_ERROR_OVERFLOW;
    }
    if (isnan(*res)) {
	    return OPT_ERROR_NAN;
    }
    return OPT_SUCCESS;
}

errorCode fastPow(double *result, double base, int degree){
    if (degree < 0) {
		return fastPow(result, 1 / base, -degree);
	} else if (degree == 0) {
		*result = 1.0;
		return OPT_SUCCESS;
	} else if (degree % 2 == 0) {
		double temp;
		errorCode opt_status = fastPow(&temp, base * base, degree / 2.0);
		if (opt_status == OPT_SUCCESS) {
			*result = temp;
			if (isinf(*result)) {
				return OPT_ERROR_OVERFLOW;
			}else if (isnan(*result)) {
				return OPT_ERROR_NAN;
			}
		}	
		return opt_status;
	} else {
		double temp;
		errorCode opt_status = fastPow(&temp, base * base, (degree - 1.0) / 2.0);
		if (opt_status == OPT_SUCCESS) {
			*result = temp * base;
			if (isinf(*result)) {
				return OPT_ERROR_OVERFLOW;
			}else if (isnan(*result)) {
				return OPT_ERROR_NAN;
			}
		}
		return opt_status;
	}
	return OPT_FAIL;
}

int main(int argc, char *argv[]){
	double base, result; int degree;
	printf("Please enter the base: ");
	scanf("%lf", &base);
	printf("Please enter the degree: ");
	scanf("%d", &degree);
	
	switch (fastPow(&result, base, degree)) {
	case OPT_SUCCESS:
		printf("The result of the degree = %.2f\n", result);
		break;
	case OPT_ERROR_OVERFLOW:
		printf("Error: overflow detected\n");
		break;
	case OPT_ERROR_NAN:
		printf("Error: result becomes a NaN\n");
		break;
	default:
		printf("Erroe: unknown error\n");
	}

    double res;
	switch (geometricMean(&res, 8, 2.0, 4.6, 7.8, 8.0, 5.5, 3.2, 7.8, 9.0)) {
	case OPT_SUCCESS:
		printf("Geometric mean = %.2f\n", res);
		break;
	case OPT_ERROR_OVERFLOW:
		printf("Error: overflow detected\n");
		break;
	case OPT_ERROR_INVALID:
		printf("Error: invalid parameter detected\n");
		break;
	case OPT_ERROR_NAN:
		printf("Error: result becomes a NaN\n");
		break;
	default:
		printf("Erroe: unknown error\n");
	}
}