#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define N 8
#define ITER 100

int is_prime(int n) {
    if (n <= 1) {
        return 0;
    }
    if (n == 2) {
        return 1;
    }
    if (n % 2 == 0) {
        return 0;
    }

    int sqrt_n = (int)sqrt((double)n);
    for (int i = 3; i <= sqrt_n; i += 2) {
        if (n % i == 0) {
            return 0;
        }
    }

    return 1;
}

void main() {
    omp_lock_t locks[N];
    int counter[N];
    for (int i = 0; i < N; i++) {
        omp_init_lock(&locks[i]);
        counter[i] = 0;
    }
    srand(time(NULL));

    omp_set_num_threads(4);

    #pragma omp parallel
    {
        int index = 0;
        for (int i = 0; i < ITER; i++) {
            omp_set_lock(&locks[index]);
            counter[index]++;

            if (is_prime(counter[index]) && is_prime(index)) {
                printf("Primes found!: %d and %d. Resuming with same index...\n", index, counter[index]);
                continue;
            }
            omp_unset_lock(&locks[index]);
            index = rand() % N;
        }

    }

    for (int i = 0; i < N; i++) {
        printf("%d: %d\n", i, counter[i]);
    }

    for (int i = 0; i < N; i++) {
        omp_destroy_lock(&locks[i]);
    }
}