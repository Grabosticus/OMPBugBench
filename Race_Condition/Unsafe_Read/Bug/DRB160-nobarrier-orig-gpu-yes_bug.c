#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

#define LENGTH 100
#define ARR_L 16


int n;
int arr_1[ARR_L];
int arr_2[ARR_L];
int temp[ARR_L];

int main(){
  for(int i=0; i<ARR_L; i++){
    arr_1[i]=0;
    arr_2[i]=2;
    temp[i]=0;
  }
  n=2;

  #pragma omp target map(tofrom:arr_1[0:ARR_L]) map(to:arr_2[0:ARR_L],temp[0:ARR_L],n) device(0)
  {
    #pragma omp teams
    for(int i=0; i<LENGTH; i++){
      #pragma omp distribute
      for(int i=0; i<ARR_L; i++){
        temp[i] = arr_1[i] + arr_2[i];
      }

      #pragma omp distribute
      for(int i=ARR_L-1; i>=0; i--){
        arr_1[i] = temp[i] * n;
      }
    }
  }

  int val = 0;

  for(int i=0; i<LENGTH; i++) {
    val = val + 2;
    val = val * 2;
  }

  for(int i=0; i<ARR_L; i++) {
    printf("index: %d val: %d\n",i, arr_1[i]);
  }

  return 0;
}