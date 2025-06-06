#include <stdio.h>

int maximum = 10, length = 0;
unsigned operator = 0;

int main()
{
#pragma omp parallel sections shared(length, maximum) firstprivate(operator) num_threads(2)
  {
#pragma omp section
    while (1)
    {
#pragma omp critical(A)
      {
        if (length < maximum)
        {
          length++;
          printf("Length: %d\n", length);
          fflush(stdout);
        }
      }
      for (int i = 0; i < 1000; i++)
        operator = (operator + 1) % 10;
    }
#pragma omp section
    while (1)
    {
#pragma omp critical(B)
      {
        if (length > 0)
        {
          length--;
          printf("Length: %d\n", length);
          fflush(stdout);
        }
      }
      for (int i = 0; i < 1000; i++)
        operator = (operator + 1) % 10;
    }
  }
}