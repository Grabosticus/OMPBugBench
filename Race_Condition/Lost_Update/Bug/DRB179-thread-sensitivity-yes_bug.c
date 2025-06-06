#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[]) {

  int *arr; 
  int length = 100;

  arr = (int*) malloc(sizeof(int) * length);

  
#pragma omp parallel for shared(arr)
  for(int i = 0; i < length; i++) {
    arr[i] = i;
    if (i == 1) 
    { 
      arr[0] = 1; 
    }
  }

  free(arr);
  return 0;
}