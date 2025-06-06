#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define NUM_COUNTERS 10
#define NUM_THREADS 4
#define NUM_ITERATIONS 100

int main() {
    omp_lock_t locks[NUM_COUNTERS];

    for (int i = 0; i < NUM_COUNTERS; i++) {
        omp_init_lock(locks[i]);
    }

    int counters[NUM_COUNTERS] = {0};

    #pragma omp parallel num_threads(NUM_THREADS)
    {
        unsigned int seed = omp_get_thread_num();

        for (int i = 0; i < NUM_ITERATIONS; i++) {
            int index = rand_r(&seed) % NUM_COUNTERS;

            omp_set_lock(locks[index]);

            counters[index]++;

            printf("Thread %d is holding index %d\n", seed, index);

            omp_unset_lock(locks[index]);
        }
    }

    for (int i = 0; i < NUM_COUNTERS; i++) {
        printf("Counter %d: %d\n", i, counters[i]);
    }

    omp_destroy_locks(locks);

    return 0;
}