/*
Modifies the three arrays based on the values in the other arrays. Demonstrates the use of dependencies for the calculations of arr2
Bug Lines: 25, 27
Difficulty: Easy
*/

#include <stdio.h>
#include <omp.h>

float incr(int i) {
    return i++;
}

float mult(float a, float b) {
    return a * b;
}

float div(float b) {
    return b / 8.0;
}

void operate( int N, float *arr1, float *arr2, float *arr3 )
{
  int i;

  #pragma omp for ordered(1)
  for (i=1; i<N; i++)
  {
    arr1[i] = incr(i);

  #pragma omp ordered depend(sink: i-1)
    arr2[i] = mult(arr1[i], arr2[i-1]);
  #pragma omp ordered depend(source)

    arr3[i] = div(arr2[i]);
  }
}

void main() {
    int N = 2;
    float arr1[2] = {1, 2};
    float arr2[2] = {5, 6};
    float arr3[2] = {8, 9};
    operate(N, arr1, arr2, arr3);
    printf("ARR1: %f %f\n", arr1[0], arr1[1]);
    printf("ARR2: %f %f\n", arr2[0], arr2[1]);
    printf("ARR3: %f %f\n", arr3[0], arr3[1]);
}