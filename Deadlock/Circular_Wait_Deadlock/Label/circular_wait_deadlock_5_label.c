/*
Calculates values in a shared array. The order of which sum gets calculated first doesn't matter.
Bug: Circular Wait of length N. The threads lock the index with their own id, and the index with (id + 1). The last thread
locks their own index and index 0. --> Deadlock
Difficulty: Medium
*/

#include <stdio.h>
#include <omp.h>

#define N 4

omp_lock_t locks[N];
int vals[N] = {0};

int get_next(int val) {
    return (val + 1) % N;
}

void func() {
    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        int next = get_next(tid);
        if (next < tid) { // global lock ordering to avoid deadlock
            int tmp = tid;
            tid = next;
            next = tmp;
        }
        omp_set_lock(&locks[tid]);
        omp_set_lock(&locks[next]);
        vals[tid] += vals[next] + 1;
        omp_unset_lock(&locks[next]);
        omp_unset_lock(&locks[tid]);
    }
}

void main() {
    omp_set_num_threads(N);
    for (int i = 0; i < N; i++) {
        omp_init_lock(&locks[i]);
    }

    func();

    for (int i = 0; i < N; i++) {
        printf("%d: %d\n", i, vals[i]);
    }

    for (int i = 0; i < N; i++) {
        omp_destroy_lock(&locks[i]);
    }
}