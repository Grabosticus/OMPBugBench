#include <stdlib.h>  
#include <stdio.h>

int main(int argc, char* argv[])  
{
  int i;
  int l = 100;

  int numNodes_0 = l, numNodes_1 = 0; 
  int arr[100]; 

  for (i = 0; i < l; i++)
  {
    if (i%2==0)
      arr[i]=5;
    else
      arr[i]= -5;
  }

#pragma omp parallel for
  for (i = numNodes_0 - 1; i > -1; --i) {
    if (arr[i] <= 0) {
      numNodes_1--;
    }
  }
  printf ("numNodes_1: %d\n", numNodes_1);
  return 0;
} 