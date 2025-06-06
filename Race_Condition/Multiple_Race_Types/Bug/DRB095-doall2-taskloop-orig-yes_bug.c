#include <stdio.h>

#define LENGTH 75

int arr[75][75];
int main()
{
  int i, j;
#pragma omp parallel
  {
#pragma omp single
    {
#pragma omp taskloop
      for (i = 0; i < LENGTH; i++)
        for (j = 0; j < LENGTH; j++)
          arr[i][j] += 1; 
    }
  }
  printf ("arr[30][30]: %d\n", arr[30][30]);
  return 0;
}