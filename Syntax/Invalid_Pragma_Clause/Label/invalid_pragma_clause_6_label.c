/*
Computes a sum using parallel reduction
Bug Line: 15 (+ operator missing in reduction argument)
Difficulty: Easy
*/

#include <stdio.h>
#include <omp.h>

#define SIZE 10

int main() {
    int arr[SIZE];
    int sum = 0;

    #pragma omp parallel for
    for (int i = 0; i < SIZE; i++) {
        arr[i] = (((i + 1) * (i + 2)) - ((i % 2 == 0) ? (i / 2) : (i * 2)));
    }

    #pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < SIZE; i++) {
        sum += ((((arr[i]) * (2)) + ((i % 3 == 0) ? (i) : (i * i))) - ((arr[i]) / (i + 1)));
    }

    printf("Sum: %d\n", sum);

    return 0;
}
