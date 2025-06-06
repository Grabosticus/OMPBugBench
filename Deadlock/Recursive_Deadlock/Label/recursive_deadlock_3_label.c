/*
Performs operations on shared variables using locks.
Bug: omp_test_lock() checks if the lock is available AND LOCKS IT, if yes. Therefore, the subsequent call to op_v1() will lead to a deadlock.
Difficulty: Medium
*/

#include <stdio.h>
#include <omp.h>

omp_lock_t lock1;
omp_lock_t lock2;

int op_v1(int var1, int n) {
    omp_set_lock(&lock1);
    var1 += n;
    omp_unset_lock(&lock1);
    return var1;
}

int op_v2(int var2, int n) {
    omp_set_lock(&lock2);
    var2 += n;
    omp_unset_lock(&lock2);
    return var2;
}

int main() {
    omp_init_lock(&lock1);
    omp_init_lock(&lock2);

    int var1 = 0;
    int var2 = 0;

    #pragma omp parallel num_threads(4)
    {
        int tid = omp_get_thread_num();

        if (omp_test_lock(&lock1)) {
            omp_unset_lock(&lock1); // doesn't make much sense semantically, but is the easiest way to fix the deadlock
            var1 = op_v1(var1, tid);
            var2 = op_v2(var2, tid);
        } else {
            var2 = op_v2(var2, tid);
            var1 = op_v1(var1, tid);
        }
    }

    omp_destroy_lock(&lock1);
    omp_destroy_lock(&lock2);

    printf("var1: %d\n", var1);
    printf("var2: %d\n", var2);
    return 0;
}
