#include <stdio.h>

int main()
{
  int v1 = 0, v2 = 0;
#pragma omp parallel sections shared(v1, v2) num_threads(2)
  {
#pragma omp section
    {
      v1 = 1;
      v2 = 1;
    }
#pragma omp section
    {
      int done = 0;
      while (!done)
      {
#pragma omp atomic read seq_cst
        done = v2;
      }
      v1 = 2;
    }
  }
  printf("%d\n", v1);
}