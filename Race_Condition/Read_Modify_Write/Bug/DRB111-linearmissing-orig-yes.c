#include <stdio.h>

int main()
{
  int len=100;
  double arr1[len], arr2[len], arr3[len];
  int i,j=0;

  for (i=0;i<len;i++)
  {
    arr1[i]=((double)i)/2.0; 
    arr2[i]=((double)i)/3.0; 
    arr3[i]=((double)i)/7.0; 
  }

#pragma omp parallel for 
  for (i=0; i<len; i++)
  {
    arr3[j] += arr1[i] * arr2[i];
    j++;
  }

  printf ("arr3[50]=%f\n", arr3[50]);
  return 0;
}