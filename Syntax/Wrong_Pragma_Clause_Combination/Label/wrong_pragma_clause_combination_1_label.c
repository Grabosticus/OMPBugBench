/*
Computes a sum using parallel reduction.
Bug Line: 16 (changing order of "parallel" and "omp" disables parallelism)
Difficulty: Easy
*/

#include <stdio.h>
#include <omp.h>

#define SIZE 100

int main() {
    int i, sum = 0;
    int numbers[SIZE];

    for (i = 0; i < SIZE; i++) {

        numbers[i] = i + 1;

    }

    #pragma omp parallel for reduction(+:sum)
    for (i = 0; i < SIZE; i++) {

        sum += numbers[i];

    }

    printf("Sum of array elements: %d\n", sum);

    return 0;
}