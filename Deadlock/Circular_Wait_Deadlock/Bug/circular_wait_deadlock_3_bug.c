#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <unistd.h>
#include <time.h>

#define ARRAY_SIZE 10
#define N_THREADS 10
#define ITERATIONS 20

int shared_array[ARRAY_SIZE];
omp_lock_t locks[ARRAY_SIZE];

void work(int tid) {
    for (int i = 0; i < ITERATIONS; ++i) {
        int index = (tid * i + rand()) % ARRAY_SIZE;
        int index2 = (index + 1) % ARRAY_SIZE;

        omp_set_lock(&locks[index]);

        int val = shared_array[index];
        val += tid + i;
        shared_array[index] = val;
        omp_set_lock(&locks[index2]);
        shared_array[index2] = val + tid * 2;
        omp_unset_lock(&locks[index2]);
        omp_unset_lock(&locks[index]);
    }
}

int main() {
    srand(time(NULL));
    omp_set_num_threads(N_THREADS);

    for (int i = 0; i < ARRAY_SIZE; ++i) {
        shared_array[i] = 0;
        omp_init_lock(&locks[i]);
    }

    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        work(tid);
    }

    for (int i = 0; i < ARRAY_SIZE; ++i) {
        omp_destroy_lock(&locks[i]);
    }

    for (int i = 0; i < ARRAY_SIZE; ++i) {
        printf("%d: %d ", i, shared_array[i]);
    }
    printf("\n");

    return 0;
}