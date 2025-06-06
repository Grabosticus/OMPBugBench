#include <stdio.h>
#include <stdlib.h>

int main()
{
  int i;
  int j;

  float tmp;
  float sum = 0.0;

  int len = 100;

  float arr[len][len];
  for (i = 0; i < len; i++) {
    for (j = 0; j < len; j++) {
      arr[i][j] = 0.5;
    }
  }

#pragma omp parallel for private (tmp,i,j)
  for (i = 0; i < len; i++)
    for (j = 0; j < len; j++)
    {
      tmp = arr[i][j];
      sum = sum + tmp * tmp;
    }
  printf ("Sum: %f\n", sum); 
  return 0;
}