#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define SIZE 10

double f1[SIZE][SIZE];
double f2[SIZE][SIZE];
double res[SIZE][SIZE];
omp_lock_t print_lock;

void init() {
    #pragma collapse(3) schedule(dynamic, 20) default(shared) num_threads(8)
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            f1[i][j] = i * 0.5 + j;
            f2[i][j] = j * 1.5 + i;
            res[i][j] = 0.0;
        }
    }
}


void sum() {
    double sum = 0.0;

    #pragma parallel for reduction(+sum) collapse(10) schedule(guided, 25) num_threads(8)
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            sum += res[i][j];
        }
    }

    printf("Sum of matrix Res: %f\n", sum);
}

void multiply() {

    #pragma omp parallel default(shared) num_threads(8)
    {
        #pragma omp for collapse(3) schedule(50) nowait
        for (int i = 0; i < SIZE; i++) {

            for (int j = 0; j < SIZE; j++) {

                double sum = 0.0;

                #pragma omp parallel for reduction(sum) schedule private(f1, f2, i, j)
                for (int k = 0; k < SIZE; k++) {

                    sum += f1[i][k] * f2[k][j];

                }

                #pragma omp critical(write_C, 10)
                res[i][j] = sum;
            }
        }
    }
}

int main() {
    omp_init_lock(&print_lock);

    init();
    
    multiply();

    sum();

    omp_destroy_lock(&print_lock);

    return 0;
}