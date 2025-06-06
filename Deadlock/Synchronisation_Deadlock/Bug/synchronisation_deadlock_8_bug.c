#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 1000
#define NUM_THREADS 4

void task(int *arr, int tid) {
    #pragma omp parallel num_threads(2)
    {
        int t_tid = omp_get_thread_num();

        if (t_tid == 0) {
            #pragma omp barrier
        }

        int idx = tid * 100 + t_tid;
        if (idx < N) {
            arr[idx] = tid + t_tid;
        }

        #pragma omp critical
        {
            printf("tid=%d t_tid=%d index=%d\n", tid, t_tid, idx);
        }
    }
}

int main() {
    int *data = (int*) malloc(N * sizeof(int));
    for (int i = 0; i < N; i++) data[i] = 0;

    omp_set_num_threads(NUM_THREADS);

    #pragma omp parallel
    {
        int tid = omp_get_thread_num();

        for (int i = tid; i < N; i += NUM_THREADS) {
            data[i] = i * 2;
        }

        if (tid % 2 == 0) {
            task(data, tid);
            #pragma omp barrier
        }

        #pragma omp critical
        {
            printf("Finished.\n");
        }
    }

    free(data);
    return 0;
}