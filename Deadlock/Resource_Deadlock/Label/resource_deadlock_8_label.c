/*
Sums random array values in parallel.
Bug: The find function changes the index of the lock. Therefore the lock set on line 31 of the bug file doesn't get unset.
If another or the same thread now gets the same index for the lock, it cannot proceed.
Difficulty: Medium
*/

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 100
#define S 3
#define START_1 2
#define START_2 5
#define START_3 10

omp_lock_t locks[N];
omp_lock_t sum_lock;
int vals[N];

void find(int *val) {
    *val = rand() % N;
    printf("Found %d\n", *val);
}

void print(int sum, int i) {
    printf("Sum %d: %d\n", i, sum);
}

void calculate(int *sum, int depth) {
    if (depth > 2) {
        return;
    }
    int i = rand() % N;
    omp_set_lock(&locks[i]);
    omp_set_lock(&sum_lock);
    *sum += vals[i];
    omp_unset_lock(&sum_lock);
    omp_unset_lock(&locks[i]);
    find(&i);
    calculate(sum, depth + 1);
}

void main() {
    srand(time(NULL));
    int sums[S] = {START_1, START_2, START_3};
    omp_set_num_threads(S);
    for (int i = 0; i < N; i++) {
        omp_init_lock(&locks[i]);
        vals[i] = i;
    }
    omp_init_lock(&sum_lock);

    #pragma omp parallel shared(sums)
    {
        calculate(&sums[omp_get_thread_num()], 0);
    }

    for (int i = 0; i < N; i++)
        omp_destroy_lock(&locks[i]);
    
    omp_destroy_lock(&sum_lock);

    for (int i = 0; i < S; i++) {
        print(sums[i], i);
    }
}