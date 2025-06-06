/*
Demonstrates OpenMP task synchronization and dependencies.
Bug Lines: 8, 10 ("omp" written two times, "private(x)" written after "shared(x)"), 12 (same "private(x)" error), 14 ("taskwait" takes argument)
Difficulty: Medium
*/

#include <stdio.h>

int main()
{
   int x;

   #pragma omp parallel
   #pragma omp single
   {
      #pragma omp task shared(x) depend(out: x)
         x = 1;
      #pragma omp task shared(x) depend(out: x)
         x = 2;
      #pragma omp taskwait
      printf("x = %d\n", x);
   }
   return 0;
}