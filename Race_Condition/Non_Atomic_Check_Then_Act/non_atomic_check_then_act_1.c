/*
Performs a one time operation. If the first thread fails in doing so, the second thread performs it.
Data Race Lines: 33 vs. 18 (can be fixed by adding a lock around the one_time_operation() function and around the read of "done" in main())
Difficulty: Easy
*/

#include <stdio.h>
#include <omp.h>

int done = 0;

void one_time_operation() {
    if (done) {
        printf("Error: Operation already done.\n");
    } else {
        printf("Thread %d Performing Operation.\n", omp_get_thread_num());
        #pragma omp flush
        done = 1;
        #pragma omp flush(done)
    }
}

int main() {

#pragma omp parallel num_threads(2) shared(done)
    {
        int tid = omp_get_thread_num();

        if (tid == 0) {
            one_time_operation();
        } else {
            #pragma omp flush(done)
            if (done == 0) {
                printf("Operation not done yet.\n");
                one_time_operation();
            } else {
                printf("Operation done.\n");
            }
        }
    }

    return 0;
}
