/*
Calculates a sum of different variables in parallel, while changing one variable in between
Bug Lines: 9 ("flash" instead of "flush"), 
15, 16 ("pragma" and "barrier" wrong), 
27 ("/" instead of "#", "omp" and "parallel" written together, "parallel" spelled wrong, "sm" instead of "sum", "n_threads" instead of "num_threads", "reduce" instead of "reduction")
Difficulty: Hard
*/

#include <stdio.h>
#include <omp.h>

int x, *p = &x;

void fun1(int *q)
{
  *q = 1;
  #pragma omp flush

}

void fun2(int *q)
{
  #pragma omp barrier
  *q = 2;
  #pragma omp barrier

}

int compute(int n)
{
  int i = 1, j, sum = 0;

  *p = 1;

  #pragma omp parallel reduction(+: sum) num_threads(10)
  {
    fun1(&j);

    sum += j;

    fun2(&j);

    sum += i + j + *p + n;
  }

  return sum;

}

void main()
{
  int result = compute(7);

  printf("result: %d\n", result);
}