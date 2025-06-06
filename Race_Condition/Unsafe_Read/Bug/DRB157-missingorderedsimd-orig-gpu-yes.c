#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

#define LENGTH 100
#define START 16

int main() {
  int arr[LENGTH];

  for(int i=0; i<LENGTH; i++){
    arr[i]=0;
  }

  #pragma omp target map(tofrom:arr[0:LENGTH]) device(0)
  #pragma omp teams distribute parallel for simd safelen(START)
  for (int i=START; i<LENGTH; i++){
    arr[i] = arr[i-START]+ 1;
  }

  printf("%d\n", arr[97]);

  return 0;
}