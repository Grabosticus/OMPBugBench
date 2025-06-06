/*
Simulates the use of OpenMP barriers.
Bug: Even numbered threads have an additional barrier --> Deadlock.
Difficulty: Easy
*/

#include <stdio.h>
#include <omp.h>

int main() {
    omp_set_num_threads(4);

    #pragma omp parallel
    {
        int tid = omp_get_thread_num();

        if (tid % 2 == 0) {
            #pragma omp barrier
        } else {
            int x = tid * 2;
            #pragma omp barrier
        }
    }

    return 0;
}
