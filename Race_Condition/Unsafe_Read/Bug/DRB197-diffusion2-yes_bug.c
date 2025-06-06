#include <stdlib.h>
#include <stdio.h>

double factor = 0.2;
int size = 20, iterations = 100;

int main()
{
  double *buffer = malloc(2 * size * sizeof(double));
  double *grid[2] = {&buffer[0], &buffer[size - 2]};
  for (int idx = 1; idx < size - 1; idx++)
    grid[0][idx] = grid[1][idx] = 1.0 * rand() / RAND_MAX;
  grid[0][0] = grid[0][size - 1] = grid[1][0] = grid[1][size - 1] = 0.5;
  int current = 0;
  for (int step = 0; step < iterations; step++)
  {
#pragma omp parallel for
    for (int idx = 1; idx < size - 1; idx++)
    {
      grid[1 - current][idx] = grid[current][idx] + factor * (grid[current][idx - 1] + grid[current][idx + 1] - 2 * grid[current][idx]);
    }
    current = 1 - current;
  }
  for (int idx = 0; idx < size; idx++)
    printf("%1.2lf ", grid[current][idx]);
  printf("\n");
  free(buffer);
}