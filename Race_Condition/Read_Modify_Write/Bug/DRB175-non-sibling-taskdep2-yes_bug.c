#include <omp.h>
#include <stdio.h>

void func() {
  int var = 0;

#pragma omp parallel
  {
#pragma omp task depend(inout : var) shared(var)
    var++;
  }
  printf("var: %d\n", var);
}

int main() {
  func();

  return 0;
}