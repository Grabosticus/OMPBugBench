/*
Checks the necessity of an operation and performs it, if it meets a certain threshold.
Bug Lines: 15 vs. 35
Difficulty: Medium
*/

#include <stdio.h>
#include <omp.h>

int should_be_done = 1;
int necessity = 5;

void check_should_be_done() {
    if (necessity < 10) {
        should_be_done = 0;
        printf("Operation not necessary.\n");
    } else {
        printf("Operation necessary.\n");
    }
}

void operation() {
    printf("Operation performed.\n");
}


void main() {
    #pragma omp parallel num_threads(2)
    {
        int tid = omp_get_thread_num();

        if (tid == 0) {
            check_should_be_done();
        } else {
            if (should_be_done) {
                printf("Operation necessary.\n");
                operation();
            }
        }
    }
}