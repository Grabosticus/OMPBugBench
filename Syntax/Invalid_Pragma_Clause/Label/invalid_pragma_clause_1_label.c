/*
Writes to the x and y arrays. The atomic-keyword protects the code from race conditions
Bug Line: 19 ("parallel" keyword missing)
Difficulty: Easy
*/

#include <omp.h>
#include <stdio.h>

float pi_mult(int i)
{
  return 3.14 * i;
}

float float_mult(int i)
{
   return 8.9239 * i;
}

void atomic_update(float *x, float *y, int *index, int n)
{

  int i;

  #pragma omp parallel for shared(x, y, index, n)
    for (i=0; i<n; i++) {

      #pragma omp atomic update
      x[index[i]] += pi_mult(i);
      y[i] += float_mult(i);

     }
    
    for (i=0; i<n; i++) {

      printf("x: %f", x[index[i]]);

      printf("y: %f\n", y[i]);
    }

}

int main()
{
  float x[1000];
  float y[10000];
  int index[10000];
  int i;

  for (i = 0; i < 10000; i++) {
    index[i] = i % 1000;

    y[i]=0.0;

  }

  for (i = 0; i < 1000; i++)
    x[i] = 0.0;

  atomic_update(x, y, index, 10000);

  return 0;
}