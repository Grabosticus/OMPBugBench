/*
Performs a parallel reduction.
Bug Line: 8
Difficulty: Easy
*/

#include <omp.h>
#include <stdio.h>

int main() {
    int sum = 0;
    int array[8] = {1, 2, 3, 4, 5, 6, 7, 8};

    #pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < 8; i++) {
        sum += array[i];
        printf("Thread %d is adding element %d\n", omp_get_thread_num(), i);
    }

    printf("Sum: %d\n", sum);
    return 0;
}