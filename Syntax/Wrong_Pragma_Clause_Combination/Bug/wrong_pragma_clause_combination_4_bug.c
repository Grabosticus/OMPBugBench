#include <omp.h>
#include <stdio.h>

#define N 10

char string[N];

void work(int i) {
    string[i] = i + 56;
}

int main()
{
#pragma omp proc_bind num_threads (spread) (N) parallel
   {
      work(omp_get_thread_num());
   }
   for (int i = 0; i < N; i++) {
    printf("%c", string[i]);
   }
   printf("\n");
   return 0;
}