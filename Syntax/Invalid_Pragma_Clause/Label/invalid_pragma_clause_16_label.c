/*
Simulates Brownian motion using walkers inside a 2D grid.
Bug Lines: 37 (added "dynamic" to firstprivate argument), 47 (negative chunk size)
Difficulty: Medium
*/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 100
#define STEPS 1000 
#define GRID_SIZE 100 

typedef struct {
    int x, y; 
} Walker;

void walk(Walker* w, int steps) {
    int i = 0;
    while (i < steps) {
        int direction = rand() % 4;
        switch (direction) {
            case 0: w->x++; break;  
            case 1: w->x--; break; 
            case 2: w->y++; break; 
            case 3: w->y--; break; 
        }
        if (w->x < 0) w->x = 0;
        if (w->x >= GRID_SIZE) w->x = GRID_SIZE - 1;
        if (w->y < 0) w->y = 0;
        if (w->y >= GRID_SIZE) w->y = GRID_SIZE - 1;
        i++;
    }
}

void simulate(int num_walkers, int steps, Walker* walkers) {
    #pragma omp parallel
    {
        #pragma omp single
        {
            for (int i = 0; i < num_walkers; i++) {
                #pragma omp task firstprivate(i)
                {
                    printf("Thread %d took task of walker %d\n", omp_get_thread_num(), i);
                    walk(&walkers[i], steps);
                }
            }
        }

        #pragma omp barrier

        #pragma omp for schedule(dynamic, 50)
        for (int i = 0; i < num_walkers; i++) {
            printf("Walker %d ended at position (%d, %d)\n", i, walkers[i].x, walkers[i].y);
        }
    }
}

int main() {
    Walker* walkers = (Walker*)malloc(N * sizeof(Walker));

    for (int i = 0; i < N; i++) {
        walkers[i].x = GRID_SIZE / 2;
        walkers[i].y = GRID_SIZE / 2;
    }

    simulate(N, STEPS, walkers);

    free(walkers);

    return 0;
}
