#include <stdio.h>
#include <omp.h>

void func() {
  int v1 = 0, v2 = 2;

  #pragma omp task depend(inout: v1) shared(v1)
  v1++;

  #pragma omp task depend(in: v1) depend(inout: v2) shared(v1, v2)
  v2 -= v1;

  #pragma omp taskwait depend(in: v1)

  printf("v1: %d\n", v1);
  printf("v2: %d\n", v2);

  #pragma omp taskwait

}

int main(){
  #pragma omp parallel
  #pragma omp single
  func();

  return 0;
}
