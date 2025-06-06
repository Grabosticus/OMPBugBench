#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
  int length = 100; 

  if (argc>1)
    length = atoi(argv[1]);

  int arr[length];
  int ind;
  int v = 10;

#pragma omp parallel for 
  for (ind=0; ind < length; ind++)
  {
    arr[ind] = v;
    v = ind;
  }
  printf("v: %d, arr[0]: %d\n", v, arr[0]);    
  return 0;
} 
