#include <stdlib.h>
#include <omp.h>

int main(int argc,char *argv[])
{
  int i;
  int j;
  int in = 20; 

  if (argc > 1) {
    in = atoi(argv[1]);
  }

  double arrrr[in][in];

  for (i = 0; i < in; i++) {
    for (j = 0; j < in; j++) {
      arrrr[i][j] = 0.5; 
    }
  }

#pragma omp parallel for private(j)
  for (i = 0; i < in - 1; i += 1) {
    for (j = 0; j < in ; j += 1) {
      arrrr[i][j] += arrrr[i + 1][j];
    }
  }
  return 0;
}
