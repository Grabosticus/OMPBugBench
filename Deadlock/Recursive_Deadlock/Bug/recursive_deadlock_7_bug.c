#include <omp.h>
#include <stdio.h>

omp_lock_t lock;

void operate(int n, int *sum) {
    if (n == 0) {
        return;
    }
    #pragma omp parallel
    {
        omp_set_lock(&lock);
        *sum += omp_get_thread_num();
        omp_unset_lock(&lock);
        #pragma omp critical
        {
            operate(n - 1, sum);
        }
    }
}

void main() {
    int sum = 0;
    omp_init_lock(&lock);
    operate(5, &sum);
    omp_destroy_lock(&lock);
    printf("Sum: %d\n", sum);
}