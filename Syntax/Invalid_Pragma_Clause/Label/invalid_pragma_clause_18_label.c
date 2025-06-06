/*
Demonstrates the use of OpenMP depend statements while doing operations on arrays.
Bug Lines: 25 (removed "#pragma", removed "()" after depend --> sink: i-1 is outside argument),
27 (removed "()" after "depend" --> "source" is outside argument)
Difficulty: Easy
*/

#include <stdio.h>
#include <omp.h>

float power(int i) {
    return i * i;
}

float multiply(float a, float b) {
    return a * b;
}

float decrement(float b) {
    return b - 1;
}

void operate( int N, float *arr_power, float *arr_mult, float *arr_decr )
{
  int i;

  #pragma omp for ordered(1)
  for (i=1; i<N; i++)
  {
    arr_power[i] = power(i);

  #pragma omp ordered depend(sink: i-1)
    arr_mult[i] = multiply(arr_power[i], arr_mult[i-1]);
  #pragma omp ordered depend(source)

    arr_decr[i] = decrement(arr_mult[i]);
  }
}

void main() {
    int N = 3;
    float A[3] = {1, 2, 3};
    float B[3] = {4, 5, 6};
    float C[3] = {7, 8, 9};

    operate(N, A, B, C);

    printf("%f %f %f\n", A[0], A[1], A[2]);
    printf("%f %f %f\n", B[0], B[1], B[2]);
    printf("%f %f %f\n", C[0], C[1], C[2]);
}