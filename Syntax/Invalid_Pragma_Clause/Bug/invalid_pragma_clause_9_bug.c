#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

int main()
{
  int x;
  omp_lock_t lock;

  omp_init_lock (&lock);
  omp_set_lock (&lock);
  x = 0;

#pragma omp parallel shared
  {
    #pragma omp master
      {
        x = x + 1;
        omp_unset_lock (&lock);
      }

  }
  omp_destroy_lock (&lock);
  return 0;
}