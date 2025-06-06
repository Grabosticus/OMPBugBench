#include <stdio.h>
#include <omp.h>

int main() {

  int var1 = 0;
  int var2;

  #pragma omp parallel num_threads(2)
  {
    int num = omp_get_thread_num();
    if (num == 0) {
      #pragma omp critical
      { var1 = 10; }
      #pragma omp atomic write
      var2 = 1;
    } else {
      int tmp = 0;
      while (tmp == 0) {
        #pragma omp atomic read acquire
        tmp = var2;
    }
    #pragma omp critical
    { if (var1 != 10) printf("var1 = %d\n", var1); }
    }
  }
  return 0;
}