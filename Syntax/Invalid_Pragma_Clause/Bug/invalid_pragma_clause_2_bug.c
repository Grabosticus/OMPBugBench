#include <omp.h>
#include <stdio.h>

omp_lock_t lock;

void operation(float *a, int i, int j, int k) {
    omp_set_lock(&lock);
    a[i] += (1 + j) * (2 + k);
    omp_unset_lock(&lock);
}


int k_start, k_end, k_increment, j_start, j_end, j_increment, i_start, i_end, i_increment;


void compute(float *a)
{
    int i, j, k;
    #pragma omp for collapse(4) private(i, k, j)
    for (k=k_start; k<=k_end; k+=k_increment)
       for (j=j_start; j<=j_end; j+=j_increment)
          for (i=i_start; i<=i_end; i+=i_increment)
             operation(a,i,j,k);
}

void main() {
    omp_init_lock(&lock);
    float array[4] = {1, 2, 3, 4};
    k_start = 0;
    j_start = 0;
    i_start = 0;
    k_end = 5;
    j_end = 4;
    i_end = 3;
    k_increment = 1;
    j_increment = 1;
    i_increment = 1;

    compute(array);

    for (int i = 0; i < 4; i++) {
        printf("%f\n", array[i]);
    }
    omp_destroy_lock(&lock);
}