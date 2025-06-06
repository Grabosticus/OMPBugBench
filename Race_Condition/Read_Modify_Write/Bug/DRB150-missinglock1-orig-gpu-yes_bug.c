#include <omp.h>
#include <stdio.h>

int main() {

  omp_lock_t lock;
  int operator = 0;
  int index;
  omp_init_lock(&lock);

  #pragma omp target map(tofrom:operator) device(0)
  #pragma omp teams distribute parallel for
  for (int index=0; index<100; index++){
    omp_set_lock(&lock);
    operator++;
    omp_unset_lock(&lock);
  }

  omp_destroy_lock(&lock);

  printf("%d\n",operator);
  return 0;
}