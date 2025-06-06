#include <stdio.h>
#include <omp.h>

#define N 100

void print_out() {
    if (omp_get_thread_num() == 0) {
        printf("Hello from main thread %d\n", omp_get_thread_num());
    } else {
        printf("Hello from thread %d\n", omp_get_thread_num());
    }
    #pragma omp barrier
}

void main() {
    #pragma omp parallel
    {
        print_out();
        #pragma omp master
        {
            print_out();
        }
    }
}