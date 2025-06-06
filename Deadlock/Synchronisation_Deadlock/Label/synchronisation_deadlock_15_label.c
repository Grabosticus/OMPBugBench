/*
Prints out a message from different threads.
Bug: The master thread calls print_out() separately and then hangs at the barrier
Difficulty: Easy
*/

#include <stdio.h>
#include <omp.h>

#define N 100

void print_out() {
    if (omp_get_thread_num() == 0) {
        printf("Hello from main thread %d\n", omp_get_thread_num());
    } else {
        printf("Hello from thread %d\n", omp_get_thread_num());
    }
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