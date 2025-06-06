/*
Simulates operations on shared variables in OpenMP.
Bug: All fN() functions lock the locks in different order. To resolve all possible deadlocks, all locks need to be set in the same order globally.
Difficulty: Medium
*/


#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <math.h>

#define N 3

int var = 0;
double var2 = 0.0;
omp_lock_t print_lock, var_lock, var2_lock;

void f1() {
    omp_set_lock(&var_lock);

    for (int i = 0; i < N; i++) {
        var++;
    }

    #pragma omp flush

    omp_set_lock(&var2_lock);
    for (int i = 0; i < N; i++) {
        var2 += sqrt(omp_get_thread_num() + 1);
    }

    #pragma omp flush

    omp_set_lock(&print_lock);

    for (int i = 0; i < N; i++) {
        printf("%d: %d\n", omp_get_thread_num(), time(NULL));
    }

    omp_unset_lock(&print_lock);
    omp_unset_lock(&var2_lock);
    omp_unset_lock(&var_lock);
}

void f2() {
    omp_set_lock(&var_lock);

    for (int i = 0; i < N; i++) {
        var += 2;
    }

    #pragma omp flush

    omp_set_lock(&var2_lock);
    for (int i = 0; i < N; i++) {
        var2 += sqrt(omp_get_thread_num() + 2);
    }

    #pragma omp flush

    omp_set_lock(&print_lock);

    for (int i = 0; i < N; i++) {
        printf("%d: %d\n", omp_get_thread_num(), time(NULL));
    }

    omp_unset_lock(&print_lock);
    omp_unset_lock(&var2_lock);
    omp_unset_lock(&var_lock);
}

void f3() {
    omp_set_lock(&var_lock);

    for (int i = 0; i < N; i++) {
        var += 3;
    }

    #pragma omp flush

    omp_set_lock(&var2_lock);
    for (int i = 0; i < N; i++) {
        var2 += sqrt(omp_get_thread_num() + 3);
    }

    #pragma omp flush

    omp_set_lock(&print_lock);

    for (int i = 0; i < N; i++) {
        printf("%d: %d\n", omp_get_thread_num(), time(NULL));
    }

    omp_unset_lock(&print_lock);
    omp_unset_lock(&var2_lock);
    omp_unset_lock(&var_lock);
}

void f4() {
    omp_set_lock(&var_lock);

    for (int i = 0; i < N; i++) {
        var += 4;
    }

    #pragma omp flush

    omp_set_lock(&var2_lock);
    for (int i = 0; i < N; i++) {
        var2 += sqrt(omp_get_thread_num() + 4);
    }

    #pragma omp flush

    omp_set_lock(&print_lock);

    for (int i = 0; i < N; i++) {
        printf("%d: %d\n", omp_get_thread_num(), time(NULL));
    }

    omp_unset_lock(&print_lock);
    omp_unset_lock(&var2_lock);
    omp_unset_lock(&var_lock);
}


void f5() {
    omp_set_lock(&var_lock);

    for (int i = 0; i < N; i++) {
        var += 5;
    }

    #pragma omp flush

    omp_set_lock(&var2_lock);
    for (int i = 0; i < N; i++) {
        var2 += sqrt(omp_get_thread_num() + 5);
    }

    #pragma omp flush

    omp_set_lock(&print_lock);

    for (int i = 0; i < N; i++) {
        printf("%d: %d\n", omp_get_thread_num(), time(NULL));
    }

    omp_unset_lock(&print_lock);
    omp_unset_lock(&var2_lock);
    omp_unset_lock(&var_lock);
}

void f6() {
    omp_set_lock(&var_lock);

    for (int i = 0; i < N; i++) {
        var += 6;
    }

    #pragma omp flush

    omp_set_lock(&var2_lock);
    for (int i = 0; i < N; i++) {
        var2 += sqrt(omp_get_thread_num() + 6);
    }

    #pragma omp flush

    omp_set_lock(&print_lock);

    for (int i = 0; i < N; i++) {
        printf("%d: %d\n", omp_get_thread_num(), time(NULL));
    }

    omp_unset_lock(&print_lock);
    omp_unset_lock(&var2_lock);
    omp_unset_lock(&var_lock);
}

int main() {

    omp_init_lock(&print_lock);
    omp_init_lock(&var_lock);
    omp_init_lock(&var2_lock);

    #pragma omp parallel num_threads(6)
    {
        int tid = omp_get_thread_num();

        switch(tid) {
            case 0: 
                f1();
                break;
            case 1:
                f2();
                break;
            case 2:
                f3();
                break;
            case 3:
                f4();
                break;
            case 4:
                f5();
                break;
            case 5:
                f6();
                break;

        }
    }

    omp_destroy_lock(&print_lock);
    omp_destroy_lock(&var_lock);
    omp_destroy_lock(&var2_lock);

    return 0;
}
