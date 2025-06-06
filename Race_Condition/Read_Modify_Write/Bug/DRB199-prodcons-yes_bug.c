#include <stdio.h>

int producers = 4;
int consumers = 4;
int capacity = 5;
int size = 0;
int packs = 1000;

int main()
{
  int n = producers + consumers;
#pragma omp parallel for shared(size, capacity, producers, consumers, n) firstprivate(packs) num_threads(n)
  for (int i = 0; i < n; i++)
  {
    if (i < producers)
      while (packs)
      {
#pragma omp critical(One)
        if (size < capacity)
        {
          size++;
          packs--;
          printf("%d: size: %d\n", i, size);
          fflush(stdout);
        }
      }
    else
      while (packs)
      {
#pragma omp critical(Two)
        if (size > 0)
        {
          size--;
          packs--;
          printf("%d: size: %d\n", i - producers, size);
          fflush(stdout);
        }
      }
  }
}