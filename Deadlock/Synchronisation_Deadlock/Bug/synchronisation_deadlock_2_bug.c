#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N_AGENTS 4
#define N_ROUNDS 5

int main() {
    int board[N_AGENTS] = {0};

    omp_set_num_threads(N_AGENTS);

    #pragma omp parallel shared(board)
    {
        int tid = omp_get_thread_num();
        int printer = -1;

        for (int round = 0; round < N_ROUNDS; ++round) {
            int change = (tid + 1) * (round + 1);
            board[tid] += change;

            printf("%d, %d: score += %d = %d\n", tid, round, change, board[tid]);

            #pragma omp barrier

            #pragma omp single
            {
                printer = omp_get_thread_num();
                printf(">> Printer: %d\n", printer);
                printf(">> End of Round %d: Board = [", round);
                for (int i = 0; i < N_AGENTS; ++i)
                    printf("%d%s", board[i], (i < N_AGENTS - 1) ? ", " : "");
                printf("]\n\n");
            }

            if (tid != printer) {
                #pragma omp barrier
            }
        }
    }

    printf("Final board state:\n");
    for (int i = 0; i < N_AGENTS; ++i)
        printf("%d final score: %d\n", i, board[i]);

    return 0;
}