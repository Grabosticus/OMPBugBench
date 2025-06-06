#include <stdlib.h>

int main()
{
  int i;
  int tmp;
  int len = 100;

  int arr[len];
  for (i = 0; i < len; i++)
    arr[i] = i;

#pragma omp parallel for
  for (i = 0; i < len; i++)
  {
    tmp = arr[i] + i;
    arr[i] = tmp;
  }
  return 0;
}