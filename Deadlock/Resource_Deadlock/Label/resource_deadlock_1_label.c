/*
Producer-Consumer Pipeline with multiple stages.
Bug: Thread 2 acquires lock2 before waiting for the result from Thread 1. Thread 1, however, needs lock2 to write aforementioned result --> Deadlock.
Difficulty: Medium
*/

#include <stdio.h>
#include <omp.h>

int main() {
    int stage1_val = 0;
    int stage2_val = 0;
    int val = 0;

    int flag1 = 0;
    int flag2 = 0;
    int flag3 = 0;

    omp_lock_t lock1, lock2, lock3;
    omp_init_lock(&lock1);
    omp_init_lock(&lock2);
    omp_init_lock(&lock3);

    omp_set_num_threads(4);

    #pragma omp parallel shared(stage1_val, stage2_val, val, flag1, flag2, flag3, lock1, lock2, lock3)
    {
        int tid = omp_get_thread_num();

        if (tid == 0) {
            int data = 5;
            omp_set_lock(&lock1);
            stage1_val = data;
            flag1 = 1;
            printf("P: %d\n", data);
            #pragma omp flush(stage1_val, flag1)
            omp_unset_lock(&lock1);
        } else if (tid == 1) {
            while (!flag1) {
                #pragma omp flush(flag1)
            }

            omp_set_lock(&lock1);
            #pragma omp flush(stage1_val)
            int temp = stage1_val * 2;
            omp_unset_lock(&lock1);

            omp_set_lock(&lock2);
            stage2_val = temp;
            flag2 = 1;
            printf("1: %d\n", temp);
            #pragma omp flush(stage2_val, flag2)
            omp_unset_lock(&lock2);
        } else if (tid == 2) {
            while (!flag2) {
                #pragma omp flush(flag2)
            }

            omp_set_lock(&lock2);
            #pragma omp flush(stage2_val)
            int temp = stage2_val + 10;
            omp_unset_lock(&lock2);

            omp_set_lock(&lock3);
            val = temp;
            flag3 = 1;
            printf("2: %d\n", temp);
            #pragma omp flush(val, flag3)
            omp_unset_lock(&lock3);
        } else if (tid == 3) {
            while (!flag3) {
                #pragma omp flush(flag3)
            }

            omp_set_lock(&lock3);
            #pragma omp flush(val)
            printf("C: %d\n", val);
            omp_unset_lock(&lock3);
        }
    }

    omp_destroy_lock(&lock1);
    omp_destroy_lock(&lock2);
    omp_destroy_lock(&lock3);

    return 0;
}
