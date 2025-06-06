/*
Calculates a sum in parallel and does array operations.
Bug: The barriers at line 20 and line 32 in the bug file both produce a deadlock. The first one is in a critical-block, so only one thread
can reach it and the second one is in a for-block.
Difficulty: Medium
*/

#include <stdio.h>
#include <omp.h>

#define N 100

int sum = 0;
int v = 0;
omp_lock_t lock;
int vals[N] = {0};

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

void produce(int n) {
    #pragma omp parallel
    {
        omp_set_lock(&lock);
        for (int i = 0; i < n; i++) {
            vals[i] += omp_get_thread_num();
        }
        omp_unset_lock(&lock);
    }
}

void fun() {
    omp_init_lock(&lock);
    #pragma omp parallel
    {
        printf("Thread %d\n", omp_get_thread_num());
        #pragma omp critical
        {
            printf("Current master Thread %d\n", omp_get_thread_num());
            for (int i = 0; i < 2*N; i++) {
                v += omp_get_thread_num();
            }
            func();
            printf("v: %d\n", v);
        }

        #pragma omp for
        for (int i = 0; i < N; i++) {
            produce(i);
        }
    }
    omp_destroy_lock(&lock);
}

void main() {
    fun();
}