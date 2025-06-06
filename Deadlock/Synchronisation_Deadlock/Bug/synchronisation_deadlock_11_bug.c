#include <stdio.h>
#include <omp.h>

#define N 10
int sum = 0;
int v = 0;
omp_lock_t lock;
omp_lock_t result_locks[N];
int results[N] = {0}; 

void func() {
    int tid = omp_get_thread_num();
    for (int i = 0; i < N; i++) {
        int temp = i * i;

        omp_set_lock(&lock);
        sum += temp;

        if (sum % 8 == 0 && sum % 3 != 0) {
            printf("Thread %d: %d at i = %d\n", tid, sum, i);
        }
        results[i] = sum;
        omp_unset_lock(&lock);

        omp_set_lock(&result_locks[i]);
        results[i] += tid;
        omp_unset_lock(&result_locks[i]);
    }

    #pragma omp barrier

    printf("Final sum = %d\n", sum);
}

void fun() {
    omp_init_lock(&lock);
    for (int i = 0; i < N; i++) {
        omp_init_lock(&result_locks[i]);
    }
    #pragma omp parallel
    {
        #pragma omp for
        for (int i = 0; i < N; i++) {
          func();
        }
    }
    omp_destroy_lock(&lock);
    for (int i = 0; i < N; i++) {
        omp_destroy_lock(&result_locks[i]);
    }
}

void main() {
    fun();
}