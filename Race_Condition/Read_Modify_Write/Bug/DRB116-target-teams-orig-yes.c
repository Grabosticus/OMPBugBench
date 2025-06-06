#include <stdio.h>

int main(int argc, char* argv[])
{
  int i;
  int len = 100;
  double arr[len];

  for (i=0; i<len; i++)
    arr[i]= ((double)i) / 3.0;

#pragma omp target map(tofrom: arr[0:len]) 
#pragma omp teams num_teams(2) 
  {
    arr[50] *= 3.0;
  }

  printf ("arr[50]=%f\n", arr[50]);
  return 0;
}