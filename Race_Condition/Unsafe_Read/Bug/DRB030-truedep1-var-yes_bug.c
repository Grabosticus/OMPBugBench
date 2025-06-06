#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
  int i;
  int length = 100;

  if (argc>1)
    length = atoi(argv[1]);

  int pp[length];
  for (i=0; i < length; i++)
    pp[i] = i;

#pragma omp parallel for
  for (i=0; i < length-1; i++)
    pp[i+1] = pp[i] + 1;

  for (i = 0; i < length; i++) {
    printf("%d ", pp[i]);
  }
  printf("\n");

  return 0;
}