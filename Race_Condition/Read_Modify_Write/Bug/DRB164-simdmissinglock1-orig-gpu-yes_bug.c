#include <stdio.h>
#include <omp.h>

#define LENGTH 100
#define ARRAY_LENGTH 64

int main() {

  int arr[ARRAY_LENGTH];

  for(int i=0; i<ARRAY_LENGTH; i++){
    arr[i]=0;
  }

  #pragma omp target map(tofrom:arr[0:ARRAY_LENGTH]) device(0)
  #pragma omp teams distribute parallel for
  for (int i=0; i<LENGTH; i++){
    #pragma omp simd
    for(int i=0; i<ARRAY_LENGTH; i++){
      arr[i]++;
    }
  }

  printf("%d\n",arr[63]);

  return 0;
}