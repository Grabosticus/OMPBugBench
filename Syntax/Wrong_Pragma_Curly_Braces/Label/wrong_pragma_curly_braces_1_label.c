/* 
Computes a sum in parallel using lastprivate 
Bug Lines: 14 and 29
Difficulty: Easy
*/

#include <stdio.h>
#include <math.h>
#include <omp.h>
 
int   P[1000];
float A[1000];
 
float compute(float *arr)
{

  float private;
  int i;
#pragma omp simd lastprivate(private)
  for (i = 0; i < 999; ++i) {

    int j = P[i];
 
    private = 0.5f;

    if (j % 2 == 0) {
      private = A[j+1] + arr[i];
    }

    A[j] = private * 1.5f;

    private = private + A[j];
  }
  return private;
}
 
int main(void)
{
  float private, arr[1000];
  int i;
 
  for (i = 0; i < 1000; ++i) {

     P[i]   = i;
     A[i]   = i * 1.5f;
     arr[i] = i * 1.8f;

  }

  private = compute(&arr[0]);

  if (private == 8237.25) {
    printf("Success: private = %7.2f ( == 8237.25) \n", private);

  } else {
    printf("Failure: private = %7.2f ( != 8237.25) \n", private);

  }

  return 0;
}