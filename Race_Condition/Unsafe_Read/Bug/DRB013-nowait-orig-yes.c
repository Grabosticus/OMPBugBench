#include <stdio.h>

int main()
{
  int i,error;
  int len = 1000;
  int array[len], val = 5;

  for (i=0; i<len; i++)
    array[i]= i;
 
#pragma omp parallel shared(val, error)
  {
#pragma omp for nowait
    for(i = 0; i < len; i++)
      array[i] = val + array[i]*5;

#pragma omp single
    error = array[9] + 1;
  }

  printf ("Error: %d\n", error);
  return 0;
}  