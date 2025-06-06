#include <stdio.h>
#include <omp.h>

int main() {

  int ind, start=1, end=4;
  int a[4] = {};

  #pragma omp simd safelen(2)
  for (ind = start; ind < end; ind++)
    a[ind] = a[ind-start] - 1.0f;

  printf("%d %d %d %d\n", a[0], a[1], a[2], a[3]);
  return 0;
}