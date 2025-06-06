#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>

#define N 100
#define PHASES 5
#define THREADS 4

double current_grid[N][N];
double updated_grid[N][N];
omp_lock_t cell_locks[N][N];

void initialize_grid() {
    for (int row = 0; row < N; ++row)
        for (int col = 0; col < N; ++col)
            current_grid[row][col] = (row + col) % 50 + 10;
}

void copy_grid() {
    for (int row = 0; row < N; ++row)
        for (int col = 0; col < N; ++col)
            current_grid[row][col] = updated_grid[row][col];
}

void simulate_phase(int phase) {
    #pragma omp parallel num_threads(THREADS)
    {
        #pragma omp for collapse(2)
        for (int row = 0; row < N; ++row)
            for (int col = 0; col < N; ++col)
                omp_init_lock(&cell_locks[row][col]);

        #pragma omp barrier

        #pragma omp for collapse(2) schedule(dynamic)
        for (int row = 1; row < N - 1; ++row) {
            for (int col = 1; col < N - 1; ++col) {
                double neighbor_sum = 0.0;
                neighbor_sum += current_grid[row - 1][col];
                neighbor_sum += current_grid[row + 1][col];
                neighbor_sum += current_grid[row][col - 1];
                neighbor_sum += current_grid[row][col + 1];
                neighbor_sum += current_grid[row][col];

                double new_value = neighbor_sum / 5.0;

                int adj_col = 0;
                if (omp_get_thread_num() % 2 == 0) {
                    adj_col = (col - 1 + N) % N;
                } else {
                    adj_col = (col + 1) % N;
                }

                omp_set_lock(&cell_locks[row][col]);
                updated_grid[row][col] = new_value + sin(phase * 0.1);
                omp_set_lock(&cell_locks[row][adj_col]);
                updated_grid[row][adj_col] = new_value * sin(phase * 0.1);
                omp_unset_lock(&cell_locks[row][adj_col]);
                omp_unset_lock(&cell_locks[row][col]);
            }
        }

        #pragma omp barrier

        double thread_sum = 0.0;
        #pragma omp parallel for reduction(+:thread_sum) collapse(2)
        for (int row = 0; row < N; ++row)
            for (int col = 0; col < N; ++col)
                thread_sum += updated_grid[row][col];

        #pragma omp single
        {
            double average = thread_sum / (N * N);
            printf("Phase %d: Average Temp = %.2f\n", phase, average);
        }

        #pragma omp for collapse(2)
        for (int row = 0; row < N; ++row)
            for (int col = 0; col < N; ++col)
                omp_destroy_lock(&cell_locks[row][col]);
    }

    copy_grid();
}

int main() {
    initialize_grid();

    for (int p = 0; p < PHASES; ++p) {
        simulate_phase(p);
    }

    printf("Simulation completed.\n");
    return 0;
}