#include <stdio.h>
#include <omp.h>
#include <assert.h>

omp_lock_t lockA, lockB, lockC;
const int limit = 2;
int value = 1;

void sync_init()
{
  omp_init_lock(&lockA);
  omp_init_lock(&lockB);
  omp_init_lock(&lockC);
}

void sync_destroy()
{
  omp_destroy_lock(&lockA);
  omp_destroy_lock(&lockB);
  omp_destroy_lock(&lockC);
}

void sync_start(int thread_id)
{
  if (thread_id == 0)
  {
    omp_set_lock(&lockA);
    omp_set_lock(&lockC);
  }
  else if (thread_id == 1)
  {
    omp_set_lock(&lockB);
  }
}

void sync_stop(int thread_id)
{
  if (thread_id == 0)
  {
    omp_unset_lock(&lockA);
    omp_unset_lock(&lockC);
  }
  else if (thread_id == 1)
  {
    omp_unset_lock(&lockB);
  }
}

void sync_wait(int thread_id)
{
  if (thread_id == 0)
  {
    omp_unset_lock(&lockA);
    omp_set_lock(&lockB);
    omp_set_lock(&lockA);
    omp_unset_lock(&lockB);
  }
  else if (thread_id == 1)
  {
    omp_set_lock(&lockA);
    omp_unset_lock(&lockB);
    omp_unset_lock(&lockA);
    omp_set_lock(&lockB);
  }
}

int main()
{
  sync_init();
#pragma omp parallel num_threads(2)
  {
    int thread_id = omp_get_thread_num();
    sync_start(thread_id);
#pragma omp barrier
    for (int counter = 0; counter < limit; counter++)
    {
      printf("Thread %d: phase 1, counter=%d, value=%d\n", thread_id, counter, value);
      fflush(stdout);
      sync_wait(thread_id);
      if (thread_id == 0)
        value = 0;
      sync_wait(thread_id);
      printf("Thread %d: phase 3, counter=%d, value=%d\n", thread_id, counter, value);
      fflush(stdout);
      sync_wait(thread_id);
      if (thread_id == 1)
        value = 1;
      sync_wait(thread_id);
    }
#pragma omp barrier
    sync_stop(thread_id);
  }
  sync_destroy();
  printf("Done: value=%d\n", value);
}
