/*
Converts integers to chars and prints out the result.
Bug Line: 14
Difficulty: Medium
*/

#include <omp.h>
#include <stdio.h>

#define N 10

char string[N];

void work(int i) {
    string[i] = i + 56;
}

int main()
{
#pragma omp parallel proc_bind(spread) num_threads(N)
   {
      work(omp_get_thread_num());
   }
   for (int i = 0; i < N; i++) {
    printf("%c", string[i]);
   }
   printf("\n");
   return 0;
}