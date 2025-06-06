/*
Performs a parallel reduction.
Bug Line: 7 (inserted a invisible space in the pragma)
Difficulty: Easy
*/

#include <omp.h>
#include <stdio.h>

int main() {
    int sum = 0;

    #pragma omp parallel for reduction(+: sum)
    for (int i = 0; i < 10; i++) {
        sum += i;
    }

    printf("Sum: %d\n", sum);
    return 0;
}