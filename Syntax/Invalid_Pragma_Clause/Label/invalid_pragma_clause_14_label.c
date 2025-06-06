/*
Performs a work based on the thread number in parallel.
Bug Lines: 12 (uses 4 threads to perform work on an array with only 3 entries --> exceeds array limits)
Difficulty: Easy
*/

#include <omp.h>
#include <stdio.h>

char string[3];

void work(int i) {
    string[i] = 'B';
}

int main()
{
#pragma omp parallel proc_bind(spread) num_threads(3)
   {
      work(omp_get_thread_num());
   }
   printf("%c%c%c\n", string[0], string[1], string[2]);
   return 0;
}