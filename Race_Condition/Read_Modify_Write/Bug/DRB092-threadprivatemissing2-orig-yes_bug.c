#include <stdio.h>
#include <assert.h>
int s0 = 0;
int s2 = 0;

int main()
{
  int i, s1 = 0;
#pragma omp parallel
  {
#pragma omp for
    for (i=1;i<=1000;i++)
    {
      s0 = s0 + i;
    }   
#pragma omp critical
    {
      s1 = s1 + s0;
    } 
  }  
  for (i=1;i<=1000;i++)
  {
    s2 = s2 + i;
  }
  printf("s1: %d; s2: %d\n", s1, s2);
  return 0;
}
