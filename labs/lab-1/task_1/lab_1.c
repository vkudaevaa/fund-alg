#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <string.h>


typedef enum kOpts {
    OPT_H,
    OPT_P,
    OPT_S,
    OPT_E,
    OPT_A,
    OPT_F
} kOpts;

int GetOpts(int argc, char** argv, kOpts* option, int* number) {
    if (argc < 2) {
        return 1;
    }
    *number = atoi(argv[1]);
    if (strcmp(argv[2], "-h") == 0) {
        *option = OPT_H;
        return 0;
    }
    else if (strcmp(argv[2], "-p") == 0) {
        *option = OPT_P;
        return 0;
    }
    else if (strcmp(argv[2], "-s") == 0) {
        *option = OPT_S;
        return 0;
    }
    else if (strcmp(argv[2], "-e") == 0) {
        *option = OPT_E;
        return 0;
    }
    else if (strcmp(argv[2], "-a") == 0) {
        *option = OPT_A;
        return 0;
    }
    else if (strcmp(argv[2], "-f") == 0) {
        *option = OPT_F;
        return 0;
    }

    return 1;

}

void remove_leading_zeros(char *num) {
    int i = 0, j = 0;
    int sign = 1;
    if (num[i] == '-') {
        sign = -1;
        i++;
    }

    // Пропускаем все ведущие нули
    while (num[i] == '0') {
        i++;
    }
    // Если все символы были нулями, оставить один ноль
    if (num[i] == '\0') {
        num[0] = '0';
        num[1] = '\0';
        return;
    }
    if (sign == -1){
        num[j] == '-';
        j++;
    }
    // Сдвигаем строку влево, чтобы убрать ведущие нули
    while (num[i] != '\0') {
        num[j++] = num[i++];
    }
    num[j] = '\0';
}

void HandlerOptH(int number){
    int flag = 0; 
    if (number == 0){
        printf("Invalid number\n");
        return;
    }
    for (int i = 1; i <= 100; i++) {
        if (i % number == 0) {
            printf("%d ", i);
            flag = 1; 
        }
    }

    if (!flag) {
        printf("There are no multiples of %d within 100", number);
    }
    printf("\n");    
}

void HandlerOptP(int number){
    if (number <= 1) {
        printf("The number %d is not simple and is not composite\n", number);
        return;
         
    }

    for (int i = 2; i <= sqrt(number); i++) {
        if (number % i == 0) {
            printf("The number %d is a composite\n", number);
            return;
             
        }
    }
    printf("The number %d is prime\n", number);
}

void HandlerOptF(int number){
    if (number < 0){
        printf("There is no factorial for negative numbers\n");
        return;
    }
    if (number == 0){
        printf("%d!\n", 1);
        return;
    }
    size_t factorial = 1;
    for (int i = 1; i <= number; i++) {
        if (factorial > ULLONG_MAX / i) {
            printf("Overflow when calculating the factorial %d!\n", i);
            return; 
        }
        factorial *= i;
    }

    printf("%zu!\n", factorial);   
}

void HandlerOptS(int number){
    char res[100];
    char result[100];
    int index = 0;
    int sign;
    // Если число равно 0, возвращаем строку "0"
    if (number == 0) {
        res[0] = '0';
        res[1] = '\0';
        printf("%s\n", res);
        return;
    }
    if (number < 0) {
    sign = -1;
    }
    number = abs(number);

    while (number > 0) {
        int remainder = number % 16;
        // Преобразуем остаток в символ (0-9, A-H)
        if (remainder < 10) {
            res[index] = remainder + '0';  
        } else {
            res[index] = remainder - 10 + 'A'; 
        }
        number /= 16;
        index++;
    }
    if (sign == -1){
        res[index++] = '-';
    }
    res[index] = '\0';

    // Переворачиваем строку
    for (int i = 0; i < index; i++) {
        result[i] = res[index - 1 - i];
    }
    result[index] = '\0'; 
    remove_leading_zeros(result); 
    for (int i = 0; i < index; i++) {
        printf("%c ", result[i]);
    }
    printf("\n");
}

void HandlerOptE(int number){
    if (number > 10){
        printf("The value should be <= 10\n");
    }
    for (int x = 1; x <= number; x++) {
        printf("Degree %d: ", x);
        for (int y = 1; y <= 10; y++) {
            // if (abs((int)pow(y, x)) >= INT_MAX){
            //     printf("\nOverflow when calculating the degree\n");
            //     return;
            // }else{
                printf("%zu ", (size_t)pow(y, x));
            // }  
        }
        printf("\n");
    }
}

void HandlerOptA(int number){
    if (number <= 0){
        printf("The number %d is not a natural number\n", number);
        return;
    }
    int sum = ((1 + number) * number) / 2;
    printf("Sum = %d\n", sum);
}

int main(int argc, char** argv) {
    kOpts opt = OPT_H;
    int procceed_number = 0;
    void (*handlers[6])(int) = {
        HandlerOptH,
        HandlerOptP,
        HandlerOptS,
        HandlerOptE,
        HandlerOptA,
        HandlerOptF
    };

    if (GetOpts(argc, argv, &opt, &procceed_number)) {
        printf("%s", "Incorrect option\n");
        return 1;
    }

    handlers[opt](procceed_number);
    return 0;
}
