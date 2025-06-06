/*
Performs a matrix multiplication and prints out the result
Bug Line: 7 (collapsing the three nested loops leads to wrong output)
Difficulty: Medium
*/

#include <stdio.h>
#include <omp.h>

#define N 3

void multiply_matrices(double m1[N][N], double m2[N][N], double result[N][N]) {
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            result[i][j] = 0.0;
            for (int k = 0; k < N; k++) {
                result[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }
}

int main() {
    double m1[N][N] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    double m2[N][N] = {{9, 8, 7}, {6, 5, 4}, {3, 2, 1}};
    double result[N][N];

    multiply_matrices(m1, m2, result);

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%.2f ", result[i][j]);
        }
        printf("\n");
    }

    return 0;
}
