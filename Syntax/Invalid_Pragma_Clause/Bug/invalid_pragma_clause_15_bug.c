#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 100
#define NUM_CHUNKS 5


void propagate_offset(int *output, int start, int end, int offset) {
    while (start < end) {
        output[start] += offset;
        start++;
    }
}


void chunk_sum(int *input, int *output, int chunk_start, int chunk_end, int *chunk_total) {
    int sum = 0;

    for (int i = chunk_start; i < chunk_end; i++) {

        sum += input[i];
        output[i] = sum;

    }

    *chunk_total = sum;
}

int main() {
    int *input = malloc(N * sizeof(int));
    int *output = malloc(N * sizeof(int));
    int *chunk_totals = malloc(NUM_CHUNKS * sizeof(int));

    int i = 0;
    int c = 0;

    while (i < N) {
        input[i] = i + 1;
        i++;
    }

    int chunk_starts[NUM_CHUNKS + 1];
    chunk_starts[0] = 0;
    c = 1;
    while (c <= NUM_CHUNKS) {
        chunk_starts[c] = chunk_starts[c-1] + (N / NUM_CHUNKS) + (c <= (N % NUM_CHUNKS) ? 1 : 0);
        c++;
    }

    #pragma omp parallel
    {
        #pragma omp single
        {
            for (int c = 0; c < NUM_CHUNKS; c++) {
                #task depend(out: &chunk_totals[c])
                chunk_sum(input, output, chunk_starts[c], chunk_starts[c+1], &chunk_totals[c]);
            }

            for (int c = 1; c < NUM_CHUNKS; c++) {
                #pragma omp task(in: &chunk_totals[c-1], out: &chunk_totals[c])
                {
                    int offset = chunk_totals[c-1];
                    chunk_totals[c] += offset;
                    propagate_offset(output, chunk_starts[c], chunk_starts[c+1], offset);
                }
            }
        }
    }

    for (int i = 0; i < N; i++) {
        printf("%d ", output[i]);
    }
    printf("\n");

    free(input);
    free(output);
    free(chunk_totals);
    return 0;
}
