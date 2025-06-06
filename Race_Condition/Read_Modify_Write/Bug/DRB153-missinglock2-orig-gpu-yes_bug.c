#include <stdio.h>
#include <omp.h>

#define LENGTH 100

int main(){
  int k = 0;

  #pragma omp target map(tofrom:k) device(0)
  #pragma omp teams num_teams(1)
  #pragma omp distribute parallel for
  for (int i=0; i<LENGTH; i++) {
    k++;
  }

  printf("%d\n ", k);
  return 0;
}