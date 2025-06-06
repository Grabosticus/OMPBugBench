#include <stdlib.h>

int main(int argc, char* argv[]) 
{
  int i;
  int j;
  int M_len = 100;

  if (argc>1)
    M_len = atoi(argv[1]);

  int n = M_len, m = M_len;
  
  double M[n][m];

#pragma omp parallel for private(j)
  for (i=1;i<n;i++)
    for (j=0;j<m;j++)
      M[i][j] = M[i][j-1];

 return 0;     
}
  