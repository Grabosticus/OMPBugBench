#include <stdlib.h>
#include <stdio.h>

double *arrayOld, *arrayNew, factor = 0.2;
int size = 10, steps = 10;

int main()
{
  arrayOld = malloc(size * sizeof(double));
  arrayNew = malloc(size * sizeof(double));
  for (int idx = 1; idx < size - 1; idx++)
    arrayNew[idx] = arrayOld[idx] = 1.0 * rand() / RAND_MAX;
  arrayOld[0] = arrayOld[size - 1] = arrayNew[0] = arrayNew[size - 1] = 0.5;
  for (int time = 0; time < steps; time++)
  {
#pragma omp parallel for
    for (int idx = 1; idx < size - 1; idx++)
    {
      arrayNew[idx] = arrayOld[idx] + factor * (arrayOld[idx - 1] + arrayOld[idx + 1] - 2 * arrayOld[idx]);
    }
    double *temp = arrayOld;
    arrayOld = arrayNew;
  }
  for (int idx = 0; idx < size; idx++)
    printf("%1.2lf ", arrayOld[idx]);
  printf("\n");
  free(arrayOld);
  free(arrayNew);
}