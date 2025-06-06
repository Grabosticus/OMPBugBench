/*
Performs a parallel reduction.
Bug Line: 8 (inserted invalid Unicode character at the end of pragma statement)
Difficulty: Easy
*/

#include <omp.h>
#include <stdio.h>

int main() {
    int n = 4;
    int sum = 0;

    #pragma omp parallel for reduction(+: sum)
    for (int i = 0; i < n; i++) {
        sum += i;
    }

    printf("Sum: %d\n", sum);
    return 0;
}
