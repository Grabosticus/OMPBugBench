#include <stdio.h>
#include <omp.h>

#define SIZE 8

int main()
{
  int x, y, z;
  double temp[SIZE], grid[SIZE][SIZE][SIZE];

  for (x = 0; x < SIZE; x++) {
    for (y = 0; y < SIZE; y++) {
      for (z = 0; z < SIZE; z++) {
        grid[x][y][z] = x;
      }
    }
  }

  #pragma omp parallel for default(shared) private(y, z)
  for (x = 1; x < SIZE - 1; x++) {
    for (y = 1; y < SIZE - 1; y++) {
      for (z = 0; z < SIZE; z++) {
        temp[z] = grid[x][y - 1][z] + grid[x][y + 1][z] + grid[x - 1][y][z] + grid[x + 1][y][z];
      }
    }
  }

  for (z = 0; z < SIZE; z++) printf("%f ", temp[z]);

  printf("\n");

  return 0;
}

