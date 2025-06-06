/*
Simulates a Multi-Stage pipeline using OpenMP.
Bug: Thread 0 reaches separate barriers at line 46, 61 and 86 in the bug file, leading to a deadlock.
Difficulty: Medium
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#define N 1000000
#define NUM_THREADS 4


void do_print(int tid, double *arr, int s, int start, int end) {
    printf("Thread %d Stage %d\n", tid, s);
    for (int i = start; i < start + 5 && i < end; i++) {
         printf("%.4f ", arr[i]);
    }
    printf("\n");
}

int main() {
    double *data = (double*) malloc(N * sizeof(double));
    double *r1 = (double*) malloc(N * sizeof(double));
    double *r2 = (double*) malloc(N * sizeof(double));
    double *r = (double*) malloc(N * sizeof(double));

    for (int i = 0; i < N; i++) {
        data[i] = i;
    }

    omp_set_num_threads(NUM_THREADS);

    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        int num_threads = omp_get_num_threads();
        int chunk_size = N / num_threads;
        int start = tid * chunk_size;
        int end = (tid == num_threads - 1) ? N : start + chunk_size;

        for (int i = start; i < end; i++) {
            r1[i] = sqrt(data[i]);
        }

        #pragma omp barrier

        if (tid == 0) {
            printf("Stage 1 complete.\n");
        } else {
            do_print(omp_get_thread_num(), r1, 1, start, end);
        }

        #pragma omp barrier

        for (int i = start; i < end; i++) {
            r2[i] = log(r1[i] + 1.0);
        }

        #pragma omp barrier

        if (tid == 0) {
            printf("Stage 2 complete.\n");
        } else {
            do_print(omp_get_thread_num(), r2, 2, start, end);
        }

        #pragma omp barrier

        for (int i = start; i < end; i++) {
            r[i] = exp(r2[i]);
        }

        #pragma omp barrier

        #pragma omp critical
        {
            for (int i = start; i < start + 5 && i < end; i++) {
                printf("%.4f ", r[i]);
            }
            printf("\n");
        }

        #pragma omp barrier

        if (tid == 0) {
            printf("All stages complete.\n");
        }
    }

    free(data);
    free(r1);
    free(r2);
    free(r);

    return 0;
}