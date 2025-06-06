/*
Demonstrates the use of OpenMP barriers.
Bug: Thread 2 in the share() function doesn't wait at the barrier in the do_work() function, while all other threads do --> Deadlock
Difficulty: Medium
*/

#include <omp.h>
#include <stdio.h>

#define N 5
int data[N];

void operate(int n) {
    data[n] *= n;
}

void do_work(int n)
{
  operate(n);
  #pragma omp barrier
  operate(n);
}

void share(int k)
{
  #pragma omp parallel shared(k)
    if (omp_get_thread_num() != 2) {
        do_work(k);
    } else {
        #pragma omp barrier
    }
}

void par(int n) {
    int i;
    #pragma omp parallel private(i) shared(n)
    {
        #pragma omp for
        for (i = 0; i < n; i++) {
            share(i);
        }
    }
}

int main() {

    for (int i = 0; i < N; i++) {
        data[i] = i + 1;
    }

    par(2);
    share(2);
    do_work(2);

    for (int i = 0; i < N; i++) {
        printf("%d ", data[i]);
    }
    printf("\n");
    return 0;
}