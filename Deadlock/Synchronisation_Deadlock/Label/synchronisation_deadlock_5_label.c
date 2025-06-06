/*
Simulates a quiz with multiple rounds.
Bug: If a thread gets disqualified, it reaches a separate barrier on line 27 and doesn't reach the barriers on lines 38 and 45
Difficulty: Medium
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <omp.h>
#include <time.h>

#define N_PARTICIPANTS 5
#define N_ROUNDS 3

int main() {
    int total_time[N_PARTICIPANTS] = {0};

    srand(time(NULL));
    omp_set_num_threads(N_PARTICIPANTS);
    int disqualified[N_PARTICIPANTS] = {0};

    #pragma omp parallel shared(total_time)
    {
        int tid = omp_get_thread_num();

        for (int round = 1; round <= N_ROUNDS; round++) {

            if ((rand() % 5) == 0) {
                printf("Participant %d got disqualified in round %d.\n", tid, round);
                total_time[tid] = 999999;
                disqualified[tid] = 1;
            }

            if (!disqualified[tid]) {
                int thinking_time = 100 + rand() % 400;
                printf("Participant %d is solving question for round %d (takes %d ms)...\n", tid, round, thinking_time);
                usleep(thinking_time * 1000);
                total_time[tid] += thinking_time;

                printf("Participant %d finished round %d.\n", tid, round);
            }

            #pragma omp barrier

            #pragma omp single
            {
                printf("All participants have finished round %d!\n\n", round);
            }

            #pragma omp barrier
        }

        #pragma omp barrier

        #pragma omp single
        {
            printf("\nFinal Results:\n");
            for (int i = 0; i < N_PARTICIPANTS; ++i)
                printf("Participant %d: Total time = %d ms\n", i, total_time[i]);
        }
    }

    return 0;
}
