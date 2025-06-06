/* Computes a sum.
Data Race Lines: 17 vs. 22 (can be fixed by adding locks)
Difficulty: Medium
*/

#include <stdio.h>
#include <omp.h>

int main() {
    int done = 0;
    int sum = 0;

#pragma omp parallel num_threads(2) shared(done, sum)
    {
        int tid = omp_get_thread_num();

        if (done == 0) {
            printf("Thread %d: Doing the work...\n", tid);
            if (done) {
                printf("ERROR: Operation already done.\n");
            } else {
                done = 1;
                #pragma omp flush
            }

            #pragma omp flush
            for (volatile int i = 0; i < 1000000; ++i) {
                #pragma omp critical 
                {
                    sum += 1;
                }
            }
            #pragma omp critical
            {
                printf("Sum %d\n", sum);
            }
        } else {
            printf("Thread %d: Operation already done.\n", tid);
        }
    }

    return 0;
}
