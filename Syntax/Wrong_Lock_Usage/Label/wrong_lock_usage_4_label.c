/*
Builds a histogram in parallel.
Bug Lines: 12 (lock declared as type int instead of omp_lock_t), 15, 26, 28, 36 (no & used)
Difficulty: Easy
*/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define NUM_BINS 10
#define NUM_ELEMENTS 1000000
#define NUM_THREADS 4

int main() {
    int *data = malloc(NUM_ELEMENTS * sizeof(int));
    int histogram[NUM_BINS] = {0};
    omp_lock_t bin_locks[NUM_BINS];

    for (int i = 0; i < NUM_BINS; i++) {
        omp_init_lock(&bin_locks[i]);
    }

    #pragma omp parallel for num_threads(NUM_THREADS)
    for (int i = 0; i < NUM_ELEMENTS; i++) {
        data[i] = rand() % NUM_BINS;
    }

    #pragma omp parallel for num_threads(NUM_THREADS)
    for (int i = 0; i < NUM_ELEMENTS; i++) {
        int bin = data[i];
        omp_set_lock(&bin_locks[bin]);
        histogram[bin]++;
        omp_unset_lock(&bin_locks[bin]);
    }

    for (int i = 0; i < NUM_BINS; i++) {
        printf("Bin %d: %d\n", i, histogram[i]);
    }

    for (int i = 0; i < NUM_BINS; i++) {
        omp_destroy_lock(&bin_locks[i]);
    }
    free(data);

    return 0;
}
