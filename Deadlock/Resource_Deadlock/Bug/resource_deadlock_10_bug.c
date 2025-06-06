#include <stdio.h>
#include <omp.h>
#include <unistd.h>

omp_lock_t x_lock, y_lock;

void change(int *v, omp_lock_t l) {
  omp_set_lock(&l);
  *v += 22;
  omp_unset_lock(&l);
}

int process(int v, int b) {
  return v + b;
}

void print_out(int v, int t) {
  if (!t) {
    printf("Task %d\n", v);
  } else {
    printf("Res: %d\n", v);
  }
}

void function() {
  int x = 0, y = 1;
  
  #pragma omp parallel
  {
      #pragma omp task
      {
        print_out(1, 0);
        omp_set_lock(&x_lock);
        x += 2;
        omp_set_lock(&y_lock);
        y *= x + 2;
        omp_unset_lock(&y_lock);
        omp_unset_lock(&x_lock);
        print_out(1, 0);
      }
      
      #pragma omp task
      {
        print_out(2, 0);
        omp_set_lock(&y_lock);
        change(&y, x_lock);
        omp_unset_lock(&y_lock);
        print_out(2, 0);
      }
  }

  int res = process(x, y);
  print_out(res, 1);
}

void main() {
  omp_set_num_threads(2);
  omp_init_lock(&x_lock);
  omp_init_lock(&y_lock);
  function();
  omp_destroy_lock(&x_lock);
  omp_destroy_lock(&y_lock);
}