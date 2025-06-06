/*
Calculates a sum in parallel.
Bug: The thread in the single region calls func() and then waits forever at the barrier in the function.
Difficulty: Easy
*/

#include <stdio.h>
#include <omp.h>

#define N 100

int sum = 0;
int v = 0;
omp_lock_t lock;

void func() {
    for (int i = 0; i < N; i++) {
        omp_set_lock(&lock);
        sum += i;
        if (sum % 8 == 0) {
            printf("Sum: %d\n", sum);
        }
        omp_unset_lock(&lock);
    }
}

void fun() {
    omp_init_lock(&lock);
    #pragma omp parallel
    {
        printf("Thread %d\n", omp_get_thread_num());
        #pragma omp single
        {
            printf("Current master Thread %d\n", omp_get_thread_num());
            for (int i = 0; i < 2*N; i++) {
                v += omp_get_thread_num();
            }
            func();
            printf("v: %d\n", v);
        }
    }
    omp_destroy_lock(&lock);
}

void main() {
    fun();
}