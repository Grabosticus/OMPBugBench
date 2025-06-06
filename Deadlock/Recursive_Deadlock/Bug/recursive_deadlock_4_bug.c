#include <stdio.h>
#include <omp.h>

omp_lock_t lock;

int r_sum(int n) {
    if (n <= 0)
        return 0;

    int sum = 0;


    #pragma omp task shared(sum)
    {
        omp_set_lock(&lock);
        sum += r_sum(n - 1);
        omp_unset_lock(&lock);
    }
    #pragma omp taskwait

    omp_set_lock(&lock);
    sum += n;
    omp_unset_lock(&lock);
    return sum;
}

void main() {
    int n = 10;
    omp_init_lock(&lock);
    int sum = 0;

    #pragma omp parallel
    {
        #pragma omp single
        {
            sum = r_sum(n);
        }
    }

    omp_destroy_lock(&lock);

    printf("Sum: %d\n", sum);
}
