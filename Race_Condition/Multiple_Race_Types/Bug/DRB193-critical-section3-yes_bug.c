#include <stdio.h>
int main()
{
  int var1 = 0, var2 = 0;
#pragma omp parallel sections shared(var1, var2) num_threads(2)
  {
#pragma omp section
    {
      var1 = 1;
#pragma omp critical(A)
      {
        var2 = 1;
      }
    }
#pragma omp section
    {
      int done = 0;
      while (!done)
      {
#pragma omp critical(B)
        {
          if (var2)
            done = 1;
        }
      }
      var1 = 2;
    }
  }
  printf("%d\n", var1);
}