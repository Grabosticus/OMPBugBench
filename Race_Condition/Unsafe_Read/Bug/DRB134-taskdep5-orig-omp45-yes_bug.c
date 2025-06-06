#include <stdio.h>
#include <omp.h>

void funct() {
  int var1 = 0;
  int var2 = 2;

  #pragma omp task depend(inout: var1) shared(var1)
  var1++;

  #pragma omp task depend(in: var1) depend(inout: var2) shared(var1, var2)
  var2 -= var1;

  #pragma omp task depend(in: var1) if(0)
  {}

  printf("var1: %d\n",var1);
  printf("var2: %d\n",var2);

  #pragma omp taskwait

}

int main(){
  #pragma omp parallel
  #pragma omp single
  funct();

  return 0;
}
