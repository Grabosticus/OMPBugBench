#include <stdio.h>
#include <time.h>
#include <math.h>
#include <omp.h>
#include <stdlib.h>

omp_lock_t lck;

int calculate(int i, int n) {
    omp_set_lock(&lck);
    int res = 0;

    if (i > 0) {
        #pragma omp task
        res = calculate(i - 1, rand() % (n + 2));
    }

    #pragma omp taskwait

    omp_unset_lock(&lck);
    return res + n;
}

int main() {
    srand(time(NULL));
    omp_init_lock(&lck);

    #pragma omp parallel num_threads(2)
    {
        int result = calculate(10, 5);
        printf("Result %d: %d\n", omp_get_thread_num(), result);
    }

    omp_destroy_lock(&lck);
    return 0;
}