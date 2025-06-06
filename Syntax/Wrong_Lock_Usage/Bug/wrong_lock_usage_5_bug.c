#include <omp.h>
#include <stdio.h>

int compute() {
    printf("compute function called.\n");
    return 0;
}

int power() {
    printf("power function called.\n");
    return 1;
}

int make() {
    printf("make function called.\n");
    return 2;
}

typedef struct {
    int a,b;
    omp_nest_lock_t lck; 
} pair;


void compute_on(pair *p, int one)
{
  p->a += one;
}

void make_pair(pair *p, int two)
{
  omp_set_nest_lock(&p->lck);
  p->b += two;
  omp_unset_nest_lock(&p->lck);
}

void compute_power_pair(pair *p, int one, int two)
{
  omp_set_nest_lock(&p->lck);
  compute_on(p, one);
  make_pair(p, two);
  omp_unset_nest_lock(&p->lck);
}

void nested_lock_pair(pair *p)
{
  #pragma omp parallel sections
  {
    #pragma omp section
      compute_power_pair(p, compute(), power());
    #pragma omp section
      make_pair(p, make());
  }
}

void main() {
    pair p;
    omp_init_lock(&p.lck);
    p.a = 1;
    p.b = 2;
    nested_lock_pair(&p);
    omp_destroy_lock(&p.lck);
}