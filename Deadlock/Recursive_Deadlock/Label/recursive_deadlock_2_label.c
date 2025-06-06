/*
Simulates a calculation using random numbers.
Bug: In the calculate() function the thread locks the same lock multiple times, because of the recursive function. --> Deadlock
Difficulty: Medium
*/

#include <stdio.h>
#include <time.h>
#include <math.h>
#include <omp.h>
#include <stdlib.h>

omp_lock_t lck;

int calculate(int i, int n) {
    int res = 0;

    if (i > 0) {
        #pragma omp task
        res = calculate(i - 1, rand() % (n + 2));
    }

    #pragma omp taskwait

    return res + n;
}

int main() {
    srand(time(NULL));
    omp_init_lock(&lck);

    #pragma omp parallel num_threads(2)
    {
        omp_set_lock(&lck);
        int result = calculate(10, 5);
        printf("Result %d: %d\n", omp_get_thread_num(), result);
        omp_unset_lock(&lck);
    }

    omp_destroy_lock(&lck);
    return 0;
}
