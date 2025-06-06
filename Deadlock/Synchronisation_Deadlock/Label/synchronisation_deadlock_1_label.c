/*
Simulates a three-phase pipeline with OpenMP barriers.
Bug: If a high priority entry is found on line 54 the responsible thread skips the barrier, leading to all other threads waiting at it --> Deadlock
Difficulty: Medium
*/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>
#include <time.h>

#define N 8

void transform(int data[], int i) {
    data[i] *= rand() % (i + 1);
}

void process(int processed[], int data[], int i) {
    processed[i] = data[i] + (rand() % N);
}

int main() {
    srand(time(NULL));
    int data[N], processed[N];
    int priority_available = 1;

    for (int i = 0; i < N; i++) {
        data[i] = i + 1;
    }

    omp_set_num_threads(4);

    #pragma omp parallel
    {
        int tid = omp_get_thread_num();

        #pragma omp for
        for (int i = 0; i < N; i++) {
            printf("%d: Loading %d\n", tid, data[i]);
            transform(data, i);
        }

        #pragma omp barrier

        #pragma omp for
        for (int i = 0; i < N; ++i) {
            printf("%d: Processing %d\n", tid, data[i]);
            process(processed, data, i);
        }

        for (int i = 0; i < N; i++) {
            printf("%d\n", data[i]);
            if (data[i] == N && priority_available) {
                printf("%d: Saving high priority...\n", tid);
                priority_available = 0;
                #pragma omp barrier
                goto saving;
            }
        }

        #pragma omp barrier

        saving:
            #pragma omp for
            for (int i = 0; i < N; ++i) {
                printf("%d: Saving %d\n", tid, processed[i]);
            }
    }

    return 0;
}
