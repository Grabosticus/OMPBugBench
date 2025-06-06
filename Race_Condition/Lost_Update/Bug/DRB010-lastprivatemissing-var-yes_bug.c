#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
  int i, var;
  int length = 10000;
  if (argc > 1)
    length = atoi(argv[1]);

#pragma omp parallel for private (i) 
  for (i = 0; i < length; i++)
    var = i;
  printf("var: %d\n", var);
  return 0;
}
