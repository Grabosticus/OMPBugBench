#include <stdio.h>
#include <omp.h>

#define NUM_THREADS 4

int main() {
    int c[NUM_THREADS][NUM_THREADS] = {0};
    int d[NUM_THREADS][NUM_THREADS] = {0};

    omp_set_num_threads(NUM_THREADS);

    #pragma omp parallel shared(c, d)
    {
        int tid = omp_get_thread_num();

        for (int e = 0; e < NUM_THREADS; ++e) {
            if (e == tid + 1) continue;

            int msg = tid * 100 + e;
            c[tid][e] = msg;

            #pragma omp flush(c)

            d[tid][e] = 1;
            #pragma omp flush(d)
        }

        for (int e = 0; e < NUM_THREADS; ++e) {
            if (e == tid) continue;

            while (d[e][tid] == 0) {
                #pragma omp flush(d)
            }

            #pragma omp flush(c)
            int received = c[e][tid];
            printf("Thread %d: received message %d from thread %d\n", tid, received, e);
        }
    }

    return 0;
}