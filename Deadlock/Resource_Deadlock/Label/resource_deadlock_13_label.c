/*
Calculates two sums using OpenMP locks.
Bug: The two arrays locks_1 and locks_2 overlap --> locks_1[N - 2] = locks_2[0] and locks_1[N - 1] == locks_2[1]. 
Therefore thread 0 could hold locks_1[N - 2] and wait for locks_1[N - 1] while thread 1 holds locks_2[1] and wait for locks_2[0]
--> Deadlock
Difficulty: Medium
*/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 5
#define SIZE 10

int sumA = 0;
int sumB = 0;

int main() {
    omp_set_num_threads(2);
    omp_lock_t *locks = malloc((SIZE) * sizeof(omp_lock_t));

    if (!locks) {
        printf("Malloc failed.\n");
        return 1;
    }

    omp_lock_t *locks_1 = locks;
    omp_lock_t *locks_2 = locks + N;

    for (int i = 0; i < N; i++) {
        omp_init_lock(&locks_1[i]);
    }

    for (int i = 0; i < N; i++) {
        omp_init_lock(&locks_2[i]);
    }

    #pragma omp parallel
    {
        if (omp_get_thread_num() == 0) {
            for (int i = 0; i < N; i++) {
                omp_set_lock(&locks_1[i]);
                sumA += i;
            }
            for (int i = 0; i < N; i++) {
                omp_unset_lock(&locks_1[i]);
            }
        } else {
            for (int i = N - 1; i >= 0; i--) {
                omp_set_lock(&locks_2[i]);
                sumB += i;
            }
            for (int i = N - 1; i >= 0; i--) {
                omp_unset_lock(&locks_2[i]);
            }
        }
    }

    for (int i = 0; i < N; i++) {
        omp_destroy_lock(&locks_1[i]);
    }
    for (int i = 0; i < N; i++) {
        omp_destroy_lock(&locks_2[i]);
    }

    printf("SumA: %d, SumB: %d\n", sumA, sumB);

    free(locks);
    return 0;
}