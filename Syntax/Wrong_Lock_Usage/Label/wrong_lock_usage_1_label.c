/*
Demonstrates the use of OpenMP locks with multiple threads. Increases a counter for every thread that locked a specific index.
Bug Lines: 13, 25, 31, 39 (lock is not referenced with & and omp_destroy_locks() is used on whole array instead of for loop with omp_destroy_lock())
Difficulty: Medium
*/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define NUM_COUNTERS 10
#define NUM_THREADS 4
#define NUM_ITERATIONS 100

int main() {
    omp_lock_t locks[NUM_COUNTERS];

    for (int i = 0; i < NUM_COUNTERS; i++) {
        omp_init_lock(&locks[i]);
    }

    int counters[NUM_COUNTERS] = {0};

    #pragma omp parallel num_threads(NUM_THREADS)
    {
        unsigned int seed = omp_get_thread_num();

        for (int i = 0; i < NUM_ITERATIONS; i++) {
            int index = rand_r(&seed) % NUM_COUNTERS;

            omp_set_lock(&locks[index]);

            counters[index]++;

            printf("Thread %d is holding index %d\n", seed, index);

            omp_unset_lock(&locks[index]);
        }
    }

    for (int i = 0; i < NUM_COUNTERS; i++) {
        printf("Counter %d: %d\n", i, counters[i]);
    }

    for (int i = 0; i < NUM_COUNTERS; i++) {
        omp_destroy_lock(&locks[i]);
    }

    return 0;
}
