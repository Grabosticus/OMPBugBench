/*
Simulates a running tournament using OpenMP.
Bug: If a runner gets injured, they don't wait at the barrier.
Difficulty: Medium
*/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <unistd.h>
#include <math.h>
#include <time.h>

#define N_RUNNERS 4

int main() {
    srand(time(NULL));
    int results[N_RUNNERS] = {0};
    int injured[N_RUNNERS] = {0};

    omp_set_num_threads(N_RUNNERS);

    #pragma omp parallel shared(results)
    {
        int id = omp_get_thread_num();

        printf("Runner %d is warming up...\n", id);
        usleep(100000 + id * 50000);

        #pragma omp barrier

        printf("Runner %d starts running!\n", id);
        int time_taken = 100 + (rand() % 200);
        usleep(time_taken * 1000);
        if (rand() % 3 == 0) {
            printf("Runner %d got injured. They are exiting the race...\n", id);
            time_taken = -1;
            injured[id] = 1;
        } 
        results[id] = time_taken;

        #pragma omp barrier

        if (!injured[id]) {
            printf("Runner %d finished the race in %d ms.\n", id, time_taken);
        }

        #pragma omp barrier

        #pragma omp single
        {
            printf("Final Results:\n");
            for (int i = 0; i < N_RUNNERS; ++i) {
                if (results[i] != -1) {
                    printf("Runner %d: %d ms\n", i, results[i]);
                }
            }
        }
    }

    return 0;
}
