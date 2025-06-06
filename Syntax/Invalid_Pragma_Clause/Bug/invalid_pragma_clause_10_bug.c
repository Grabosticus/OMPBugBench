#include <omp.h>
#include <stdio.h>

void modulo3(int i, int j) {
    if ((i * j) % 3 == 0) {
        printf("%d * %d == %d\n", i, j, (i * j));
    }
}

void parallel_modulo3(int i, int n)
{
  int j;
  #pragma omp parallel default(private)
  {
    #pragma omp for
    for (j=0; j<n; j++)
      modulo3(i, j);
  }
}


void parallel_region(int n)
{
  int i;
  #pragma omp parallel default(private)
  {
    #pragma omp for
    for (i=0; i<n; i++)
      parallel_modulo3(i, n);
  }
}

void main() {
    parallel_region(10);
}