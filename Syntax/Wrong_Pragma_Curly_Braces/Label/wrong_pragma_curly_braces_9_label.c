/*
Computes Q[i][k] * Q[k][i] for all i using OpenMP Offloading and sums up the results.
Bug Lines: 6, 10, 14, 15, 16, 20, 21, 22
Difficulty: Medium
*/

#include <stdio.h>
#include <omp.h>

#define N 100

#pragma omp declare target
float Q[N][N];
float calculate(const int i, const int k)
{ return Q[i][k] * Q[k][i]; }
#pragma omp end declare target

float operation(int k) {
    float tmp = 0.0;
    #pragma omp target update to(Q)
    #pragma omp target map(tofrom: tmp)
    #pragma omp parallel for reduction(+:tmp)
    for(int i=0; i < N; i++)
        tmp += calculate(i,k);
    return tmp;
}

void main() {
    int k = 5;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            Q[i][j] = i - j;
        }
    }

    float result = operation(k);
    printf("%f\n", result);
}