/*
Performs a stencil operation on an array.
Bug Lines: 13, 19 (inserted BIDI override)
Diffiulty: Easy
*/

#include <stdio.h>
#include <omp.h>

double mult(int i, int j) {
    return (double) i * j;
}

void work(int N, int M, double (*arr1)[M], double (*arr2)[M], double (*arr3)[M])
{
  int i, j;
  double alpha = 1.2;

  #pragma omp for collapse(2) ordered(2)
  for (i = 1; i < N-1; i++)
  {
    for (j = 1; j < M-1; j++)
    {
      arr1[i][j] = mult(i, j);
  #pragma omp ordered depend(source)

      arr2[i][j] = alpha * arr1[i][j];

  #pragma omp ordered depend(sink: i-1,j) depend(sink: i,j-1)
      arr3[i][j] = arr2[i][j] * (arr1[i-1][j] + arr1[i+1][j] + arr1[i][j-1] + arr1[i][j+1] + arr1[i][j]);
    }
  }
}

void main() {
    int N = 3;
    int M = 4;
    double A[3][4] = {{0.4,2.1,1.1, 2.1},{62.0,2.0,1.345, 5.0},{1.7,0.066,8.2, 2.2}};
    double B[3][4];
    double C[3][4];
    work(N, M, A, B, C);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            printf("%f ", C[i][j]);
        }
        printf("\n");
    }
}