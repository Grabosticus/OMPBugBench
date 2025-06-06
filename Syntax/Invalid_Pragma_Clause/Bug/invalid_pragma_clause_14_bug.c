#include <omp.h>
#include <stdio.h>

char string[3];

void work(int i) {
    string[i] = 'B';
}

int main()
{
#pragma omp parallel proc_bind(spread) num_threads(4)
   {
      work(omp_get_thread_num());
   }
   printf("%c%c%c\n", string[0], string[1], string[2]);
   return 0;
}