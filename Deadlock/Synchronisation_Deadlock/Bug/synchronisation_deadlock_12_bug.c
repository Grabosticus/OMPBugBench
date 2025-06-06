#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

#define N 10
int sum = 0;
int v = 0;
omp_lock_t lock;
int nums[N] = {0};

int select() {
    int ind = rand() % N;
    return nums[ind];
}

void print_out(int sum, int sel) {
    printf("Sum %d and Selected %d\n", sum, sel);
    #pragma omp barrier
}

void operate() {
    for (int i = 0; i < N; i++) {
        omp_set_lock(&lock);
        sum += i;
        omp_unset_lock(&lock);
    }
    int sel = select();
    print_out(sum, sel);
}

void par() {
    #pragma omp parallel
    {
        #pragma omp for
        for (int i = 0; i < N; i++) {
          operate();
        }
    }
}

void main() {
    omp_init_lock(&lock);
    for (int i = 0; i < N; i++) {
        nums[i] = rand();
    }
    par();
    omp_destroy_lock(&lock);
}