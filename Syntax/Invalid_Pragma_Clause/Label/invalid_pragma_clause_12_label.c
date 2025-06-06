/*
Demonstrates the use of lastprivate in OpenMP
Bug Lines: 10 ("lastprivate(i)"" written as "last(i) private")
Difficulty: Easy
*/

#include <omp.h>
#include <stdio.h>

void test(int n, float *arr1, float *arr2)
{
  int i;

  #pragma omp parallel
  {
    #pragma omp for lastprivate(i)
    for (i=0; i<n-1; i++)
      arr1[i] = arr2[i] + arr2[i+1];
  }

  arr1[i]= arr2[i];
}

void main() {
    int n = 2;
    float a[2] = {1, 4};
    float b[2] = {8, 19};
    test(n, a, b);
    printf("a: %f %f\n", a[0], a[1]);
}