/*
Initializes matrices, multiplies them, and sums the entries of the result up in parallel.
Bug Lines: 55 (critical section given another argument, than its name), 48 (reduction() not given operation, schedule() written without argument, "shared" replaced by "private" --> leads to NaN result of sum()),
41 (collapse exceeds total number of loops, schedule only has chunk size argument),
27 (collapse exceeds loop number, reduction written without ":", "omp" not written after "#pragma"),
13 (collapse exceeds loop number, "omp parallel for" not present in pragma)
Difficulty: Hard
*/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define SIZE 10

double f1[SIZE][SIZE];
double f2[SIZE][SIZE];
double res[SIZE][SIZE];
omp_lock_t print_lock;

void init() {
    #pragma omp parallel for collapse(2) schedule(dynamic, 20) default(shared) num_threads(8)
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

    #pragma omp parallel for reduction(+:sum) collapse(2) schedule(guided, 25) num_threads(8)
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
        #pragma omp for collapse(2) schedule(static, 50) nowait
        for (int i = 0; i < SIZE; i++) {

            for (int j = 0; j < SIZE; j++) {

                double sum = 0.0;

                #pragma omp parallel for reduction(+:sum) schedule(static) shared(f1, f2, i, j)
                for (int k = 0; k < SIZE; k++) {

                    sum += f1[i][k] * f2[k][j];

                }

                #pragma omp critical(write_C)
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
