#include <stdlib.h>

int main (int argc, char* argv[])
{
  int length = 1000;
  int i; 

  if (argc>1)
    length = atoi(argv[1]);
  int arr[length];
  arr[0] = 2;

#pragma omp parallel for
  for (i = 0; i < length; i++)
    arr[i] = arr[i] + arr[0];

  return 0;
}