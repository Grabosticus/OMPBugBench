#include <stdio.h>

#define N 75

int array[N][N];

int main()
{
  int i,j;
  for (i=0; i<N; i++)
      for (j=0; j<N; j++)
          array[i][j] = 0;
#pragma omp parallel for
  for (i=0; i<N; i++)
    for (j=0; j<N; j++)
      array[i][j] = array[i][j] + N;

  for (i = 0; i < N; i++) {
    printf("%d ", array[i][0]);
  }
  printf("\n");
  return 0;
}
