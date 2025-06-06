/*
Demonstrates the use of OpenMP barriers.
Bug: Thread 1 never reaches the barrier at line 30 in the bug file. Therefore, all other threads wait there.
Difficulty: Easy
*/

#include <stdio.h>
#include <omp.h>

#define N 10

int operate() {
    int sum = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            sum += i*j;
        }
    }
    return sum;
}

int main() {
    omp_set_num_threads(4);

    #pragma omp parallel
    {
        int tid = omp_get_thread_num();

        printf("%d: Starting work...\n", tid);

        if (tid == 1) {
            int sum = operate();
            printf("%d: Sum = %d\n", tid, sum);
            #pragma omp barrier
        } else {
            printf("%d: Waiting.\n", tid);
            #pragma omp barrier
        }

        #pragma omp barrier
        printf("%d: Finished.\n", tid);
    }

    return 0;
}
