#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 5
#define SIZE 9

int sumA = 0;
int sumB = 0;

void operate(omp_lock_t *locks_1, omp_lock_t *locks_2) {
    for (int i = 0; i < N; i++) {
        omp_set_lock(&locks_1[i]);
        sumA += i + 1;
    }
    for (int i = 0; i < N - 1; i++) {
        omp_set_lock(&locks_2[i]);
        sumB += i + sumA + 1;
    }
    for (int i = 0; i < N - 1; i++) {
        omp_unset_lock(&locks_2[i]);
    }
    for (int i = 0; i < N; i++) {
        omp_unset_lock(&locks_1[i]);
    }
}

int main() {
    omp_lock_t *locks = malloc((SIZE) * sizeof(omp_lock_t));

    if (!locks) {
        printf("Malloc failed.\n");
        return 1;
    }

    omp_lock_t *locks_1 = locks;
    omp_lock_t *locks_2 = locks + (N - 1);

    for (int i = 0; i < N; i++) {
        omp_init_lock(&locks_1[i]);
    }

    for (int i = 0; i < N - 1; i++) {
        omp_init_lock(&locks_2[i]);
    }

    #pragma omp parallel
    {
        operate(locks_1, locks_2);
    }

    for (int i = 0; i < N; i++) {
        omp_destroy_lock(&locks_1[i]);
    }
    for (int i = 0; i < N - 1; i++) {
        omp_destroy_lock(&locks_2[i]);
    }

    printf("SumA: %d, SumB: %d\n", sumA, sumB);

    free(locks);
    return 0;
}