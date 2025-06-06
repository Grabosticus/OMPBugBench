#include <stdio.h>
#include <omp.h>

#define SIZE 100

int main() {
    int i, sum = 0;
    int numbers[SIZE];

    for (i = 0; i < SIZE; i++) {

        numbers[i] = i + 1;

    }

    #pragma parallel omp for reduction(+:sum)
    for (i = 0; i < SIZE; i++) {

        sum += numbers[i];

    }

    printf("Sum of array elements: %d\n", sum);

    return 0;
}