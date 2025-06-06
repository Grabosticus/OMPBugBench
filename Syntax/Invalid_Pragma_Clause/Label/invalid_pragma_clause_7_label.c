/*
Computes multiple loops in parallel with the use of nowait
Bug Lines: 10, 13, 16 ("#pragma" missing)
Difficulty: Easy
*/

#include <math.h>
#include <omp.h>
#include <stdio.h>

void compute(int n, float *arr1, float *arr2, float *res1, float *res2, float*res3)
{
   int i;
#pragma omp parallel
   {
#pragma omp for schedule(static) nowait
   for (i=0; i<n; i++)
      res1[i] = (arr1[i] + arr2[i]) / 2.0f;
#pragma omp for schedule(static) nowait
   for (i=0; i<n; i++)
      res3[i] = sqrtf(res1[i]);
#pragma omp for schedule(static) nowait
   for (i=1; i<n; i++)
      res2[i] = res3[i-1] + arr1[i];
   }
}

void main() {
    int n = 3;
    float arr1[3] = {1, 2, 3};
    float arr2[3] = {4, 5, 6};
    float res1[3] = {7, 8, 9};
    float res2[3] = {0, 0, 0};
    float res3[3] = {0, 0, 0};
    compute(n, arr1, arr2, res1, res2, res3);
    printf("%f %f %f\n", res1[0], res1[1], res1[2]);
    printf("%f %f %f\n", res2[0], res2[1], res2[2]);
    printf("%f %f %f\n", res3[0], res3[1], res3[2]);
}