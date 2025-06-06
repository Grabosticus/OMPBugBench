/*
Simulates Heat-Diffusion over a 2D grid by updating cells based on their neighbours temperature until the maximum temperature changes falls below THRESHOLD.
Bug Lines: 10, 20, 26, 44 (cyrillic characters inserted), 
60 ("private" after "shared"), 56 ("barrier" after "master"), 18, 32 (no "omp" in pragma),
26 (wrong curly brace),
63 ("for" after "taskwait"),
32 ("teams" and "parallel" spelled wrong),
65 (no "()" for flush argument)
Difficulty: Hard
*/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>

#define N 512
#define STEPS 100
#define THRESHOLD 0.001

#pragma omp declare target
double grid[N][N];
double new_grid[N][N];
#pragma omp end declare target

double _step() {
    double diff = 0.0;

    #pragma omp target data map(to: grid[:N][:N]) map(from: new_grid[:N][:N], diff)
    {
        #pragma omp target teams distribute parallel for collapse(2) reduction(max: diff)
        for (int i = 1; i < N-1; ++i) {
            for (int j = 1; j < N-1; ++j) {
                new_grid[i][j] = 0.25 * (grid[i+1][j] + grid[i-1][j] + grid[i][j+1] + grid[i][j-1]);
                double local_diff = fabs(new_grid[i][j] - grid[i][j]);
                
                #pragma omp critical 
                {
                    if (local_diff > diff) diff = local_diff;
                }
            }
        }
        
        #pragma omp target teams distribute parallel for collapse(2)
        for (int i = 1; i < N-1; ++i) {
            for (int j = 1; j < N-1; ++j) {
                grid[i][j] = new_grid[i][j];
            }
        }
    }

    return diff;
}

void _init() {
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            grid[i][j] = ((i == 0 || i == N-1 || j == 0 || j == N-1) ? 100.0 : 0.0);
}

int main() {
    _init();
    double diff = 0.0;

    #pragma omp parallel
    {
        #pragma omp master
        {
            int step = 0;
            while (step < STEPS) {
                #pragma omp task shared(diff)
                diff = _step();

                #pragma omp taskwait

                #pragma omp flush(diff)

                if (diff < THRESHOLD) {
                    printf("Converged at step %d with diff %f\n", step, diff);
                    break;
                }

                step++;
            }
        }
    }

    printf("Simulation finished.\n");
    return 0;
}
