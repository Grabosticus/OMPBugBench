/*
Computes an array with double the values of the even indices of another array
Bug Line: 16 (the stride of j is given a 2, even though it is actually 1. This leads to the limits of array2 being exceeded)
Difficulty: Easy
*/

#include <stdio.h>
#include <omp.h>

#define N 100

int main(void)
{
   float array1[N];
   float array2[N/2];

   for (int i = 0; i < N; i++ )
      array1[i] = i + 1;

   int j = 0;
   #pragma omp parallel
   #pragma omp for linear(j:1)
   for (int i = 0; i < N; i += 2 ) {
      array2[j] = array1[i] * 2.0f;
      j++;
   }

   printf("%d %f %f\n", j, array2[0], array2[j-1]);

   return 0;
}