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
        omp_set_lock(&locks[tid]);
        omp_set_lock(&locks[get_next(tid)]);
        vals[tid] += vals[get_next(tid)] + 1;
        omp_unset_lock(&locks[get_next(tid)]);
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