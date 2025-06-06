#include <stdio.h>
#include <string.h>
#include <omp.h>
#include <ctype.h>

#define MAX_STR_LEN 100

void to_uppercase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = toupper(str[i]);
    }
}

int main() {
    char strings[][MAX_STR_LEN] = {
        "hello",
        "world",
        "openmp",
        "parallel",
        "programming"
    };
    int n = 5;

    #pragma omp for parallel
    for (int i = 0; i < n; i++) {
        to_uppercase(strings[i]);
    }

    #pragma omp for parallel
    for (int i = 0; i < n; i++) {
        printf("String %d: %s\n", i, strings[i]);
    }

    return 0;
}