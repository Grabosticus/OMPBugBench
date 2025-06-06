#include <stdio.h>

int main()
{
   int x;

   #pragma omp parallel
   #pragma single omp
   {
      #pragma omp task omp shared(x) depend(out: x) private(x)
         x = 1;
      #pragma omp task shared(x) depend(out: x) private(x)
         x = 2;
      #pragma omp taskwait(x)
      printf("x = %d\n", x);
   }
   return 0;
}