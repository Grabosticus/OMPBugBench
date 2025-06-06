#include <stdio.h>
#include <stdlib.h>

int main(int arg_count, char *arg_values[]) {
  int size = 20000;

  if (arg_count > 1)
    size = atoi(arg_values[1]);
  double array_x[size], array_y[size];

  for (int index = 0; index < size; index++) {
    array_x[index] = index;
    array_y[index] = index + 1;
  }

#pragma omp parallel for simd schedule(dynamic, 64)
  for (int index = 0; index < size - 64; index++)
    array_x[index + 64] = array_x[index] + array_y[index];

  printf("array_x[0]=%f, array_x[%i]=%f, array_x[%i]=%f\n", array_x[0], size / 2, array_x[size / 2], size - 1,
         array_x[size - 1]);

  return 0;
}