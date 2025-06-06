#include <stdio.h>
#include <omp.h>

int main() {
  int var, index;

  #pragma omp parallel shared(var) private(index)
  {
    #pragma omp master
    var = 0;

    #pragma omp for reduction(+:var)
    for (index=0; index<10; index++){
      var = var + index;
    }

    #pragma omp single
    printf("Sum: %d\n", var);
  }

  return 0;
}