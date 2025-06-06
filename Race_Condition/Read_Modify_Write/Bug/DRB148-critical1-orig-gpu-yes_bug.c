#include <omp.h>
#include <stdio.h>

#define LENGTH 100

int operator = 0;

int main() {

  #pragma omp target map(tofrom:operator) device(0)
  #pragma omp teams distribute parallel for
  for(int i=0; i<LENGTH; i++){
    #pragma omp critical(addlock)
    operator++;

    #pragma omp critical(sublock)
    operator -= 2;
  }

  printf("%d\n", operator);

  return 0;
}