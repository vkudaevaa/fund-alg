#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <limits.h>

typedef enum opt {
    OPT_SUCCESS,
    OPT_VERTICES,
    OPT_CONVEX,
    OPT_NOT_CONVEX,
    OPT_FAIL,
    OPT_ERROR,
    OPT_INVALID_FORMAT,
    OPT_OVERFLOW
} opt;

typedef struct {
    const char *input;
    opt result;
} KaprekarResult;

double fast_pow(double val, int p){
    if (p < 0){ return fast_pow(1.0 / val, -p);}
    else if (p == 0){return 1.0;}
    else if (p % 2 == 0){return fast_pow(val*val, p/2);}
    else{return val * fast_pow(val * val,(p - 1) / 2);}
}

opt convex(int number, ...){
    if (number < 3) return OPT_VERTICES;
    va_list args;
    va_start(args, number);
    double x, y, vector_product;
    int sign = 0;
    double verices_x[number];
    double verices_y[number];
    for (int i = 0; i < number; i++){
        verices_x[i] = va_arg(args, double);
        verices_y[i] = va_arg(args, double);
    }
    va_end(args);
    for (int i = 0; i < number; i++){
        int next = (i + 1) % number;
        int next_next = (i + 2) % number;
        vector_product = (verices_x[next] - verices_x[i]) * (verices_y[next_next] - verices_y[next]) - (verices_y[next] 
        - verices_y[i]) * (verices_x[next_next] - verices_x[next]);
        if (i == 0) {
            sign = (vector_product > 0);
        } else {
            if ((vector_product > 0) != sign) return OPT_NOT_CONVEX; 
        }
    }
    return OPT_CONVEX;
}

double polynomial(double point, int degree, ...){
    double sum = 0.0;
    va_list args;
    va_start(args, degree);
    for (int i = degree; i >= 0; i--){
        double coef = va_arg(args, double);
        double res = fast_pow(point, i) * coef;
        sum += res;
    }
    va_end(args);
    return sum;
}

void getDigits(long long num, int base, int *digits, int *size) {
    *size = 0;
    if (num == 0) {
        digits[(*size)++] = 0;
        return;
    }
    while (num > 0) {
        digits[(*size)++] = num % base;
        num /= base;
    }

    for (int i = 0; i < *size / 2; i++) {
        int temp = digits[i];
        digits[i] = digits[*size - i - 1];
        digits[*size - i - 1] = temp;
    }
}

long long digitsToNumber(int *digits, int size, int base) {
    long long num = 0;
    for (int i = 0; i < size; i++) {
        num = num * base + digits[i];
    }
    return num;
}

opt isKaprekar(long long num, int base) {
    if (num == 1){
        return OPT_SUCCESS;
    }

    long long square = num * num;
    int digits[64];
    int size;
    getDigits(square, base, digits, &size);

    for (int split = 1; split < size; split++) {
        int leftSize = split;
        int rightSize = size - split;

        int leftDigits[32];
        int rightDigits[32];

        for (int i = 0; i < leftSize; i++) {
            leftDigits[i] = digits[i];
        }
        for (int i = 0; i < rightSize; i++) {
            rightDigits[i] = digits[split + i];
        }

        long long left = digitsToNumber(leftDigits, leftSize, base);
        long long right = digitsToNumber(rightDigits, rightSize, base);

        if (right == 0) continue;

        if (left + right == num) {
            return OPT_SUCCESS;
        }
    }

    return OPT_FAIL;
}

opt FindKaprekar(KaprekarResult *results, int base, int count, ...) {
    if (base < 2 || base > 36) {
        return OPT_ERROR;
    }

    va_list args;
    va_start(args, count);

    for (int i = 0; i < count; i++) {
        const char *numStr = va_arg(args, const char *);

        char *endptr;
        long long num = strtoll(numStr, &endptr, base);

        if (endptr == numStr || *endptr != '\0') {
            results[i].result = OPT_INVALID_FORMAT;
        } else if (num < 0) {
            results[i].result = OPT_INVALID_FORMAT;
        } else if (num == LLONG_MAX){
            results[i].result = OPT_OVERFLOW;
        }else   {
            results[i].result = isKaprekar(num, base);
        }

        results[i].input = numStr;
    }

    va_end(args);
    return OPT_SUCCESS;
}

int main(int argc, char* argv[]){
    opt res_1 = convex(5, 0.0, 0.0, 4.0, 0.0, 3.5, 2.0, 1.5, -0.5, 0.5, 2.0); // невыпуклый
    // при convex(4, 1.0, 5.0, 6.0, 9.0, 12.0, 7.0, 10.0, 1.0) выпуклый
    if (res_1 == OPT_CONVEX){
        printf("Polygon is convex\n");
    }else if (res_1 == OPT_NOT_CONVEX){
        printf("Polygon is not convex\n");
    }else if (res_1 == OPT_VERTICES){
        printf("The number of vertices must be greater than 2\n");
    }
    
    printf("Result = %.2lf\n", polynomial(5.2, 3, -14.5, 3.0, -1.0, -3.7)); // -14x^3 + 3x^2 - x - 3.7

    int count = 7;
    int base = 16;
    KaprekarResult results[count];
    opt res = FindKaprekar(results, base, count, "12", "10", "0.67", "FFFF", "7FE00", "0099999", "A");

    switch (res) {
        case OPT_SUCCESS:
            for (int i = 0; i < count; i++) {
                if (results[i].result == OPT_SUCCESS) {
                    printf("%s is Kaprekar number\n", results[i].input);
                }else if (results[i].result == OPT_INVALID_FORMAT){
                    fprintf(stderr,"%s is not valid number\n", results[i].input);
                }else if (results[i].result == OPT_OVERFLOW){
                    fprintf(stderr, "%s overflow number\n", results[i].input);
                }else{
                    printf("%s is not Kaprekar number\n", results[i].input);
                }
            }
            break;
        case OPT_ERROR:
            fprintf(stderr, "Error: Incorrect base\n");
            break;
        case OPT_INVALID_FORMAT:
            fprintf(stderr, "Error: Incorrect values\n");
            break;
    }
}