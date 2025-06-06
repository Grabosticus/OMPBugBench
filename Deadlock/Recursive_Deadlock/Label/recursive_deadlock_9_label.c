/*
Demonstrates the use of OpenMP locks.
Bug: When the same index is given two times in argv, an already acquired lock gets set again, leading to a recursive deadlock.
Difficulty: Medium
*/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define N 10

omp_lock_t locks[N];
int sum = 0;

void operate(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        omp_set_lock(&locks[arr[i]]);
        sum += n;
        omp_unset_lock(&locks[arr[i]]);
    }
}

void destroy_() {
    for (int i = 0; i < N; i++) {
        omp_destroy_lock(&locks[i]);
    }
}

int main(int argc, char **argv) {
    srand(time(NULL));
    for (int i = 0; i < N; i++) {
        omp_init_lock(&locks[i]);
    }
    int num = 4;
    if (argc > 1) {
        num = argc - 1;
    }

    int inds[num];


    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            char *val;
            int ind = strtol(argv[i], &val, 10);

            if (*val != '\0') {
                printf("Invalid digit: %s\n", argv[i]);
                destroy_();
                return 1;
            }

            if (ind > 9 || ind < 0) {
                printf("Invalid digit (must be between 0 and 9): %d\n", ind);
                destroy_();
                return 1;
            }

            inds[i - 1] = ind;
        }
    } else {
        printf("No arguments given.\n");
        destroy_();
        return 1;
    }

    #pragma omp parallel 
    {
        #pragma omp single
        {
            operate(inds, num);
        }
    }

    printf("Sum: %d\n", sum);
    destroy_();
    return 0;

}