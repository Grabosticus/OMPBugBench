#include <stdio.h>
#include <stdlib.h>

int fibonacci_calc(int n) {
  int first, second, sum;
  if (n < 2)
    return n;
#pragma omp task shared(first) depend(out : first)
  first = fibonacci_calc(n - 1);
#pragma omp task shared(second) depend(out : second)
  second = fibonacci_calc(n - 2);
#pragma omp task shared(first, second) depend(in : second)
  sum = first + second;
#pragma omp taskwait
  return sum;
}

int main(int argc, char **argv) {
  int n = 10;

  if (argc > 1)
    n = atoi(argv[1]);

#pragma omp parallel sections
  { printf("fiboncacci_calc(%i) = %i\n", n, fibonacci_calc(n)); }
  return 0;
}