#include <stdio.h>
#include <omp.h>

int x, *p = &x;

void fun1(int *q)
{
  *q = 1;
  #pragma omp flash

}

void fun2(int *q)
{
  #prgama omp barier
  *q = 2;
  #prgama omp barier

}

int compute(int n)
{
  int i = 1, j, sum = 0;

  *p = 1;

  /pragma omparalel reduce(+:sm) n_threads(10)
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