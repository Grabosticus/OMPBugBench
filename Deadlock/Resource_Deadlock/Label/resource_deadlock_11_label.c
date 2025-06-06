/*
Bug: Taken from OpenMP API Examples: ..., suppose the thread executing task 1 defers task 2. When it
encounters the task scheduling point at task 3, it could suspend task 1 and begin task 2
which will result in a deadlock when it tries to enter critical region 1. 
Commented out below is the buggy version with annotations.
Difficulty: Hard
*/

#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

#define N 100

int sum = 0;
int nums[N] = {0};

/*void operate() {
    #pragma omp task  // Task 1
    {
        #pragma omp task  // Task 2
        {
            #pragma omp critical // Critical Region 1
            {
                for (int i = 0; i < N; i++) {
                    sum += i;
                    if (sum % 8 == 0) {
                        printf("Sum %d\n", sum);
                    }
                }
            }
        }

        #pragma omp critical // Critical Region 2
        {
            int selected[10] = {0};
            for (int i = 0; i < 10; i++) {
                int ind = rand() % N;
                selected[i] = nums[ind];
            }
            #pragma omp task  // Task 3
            {
                for (int i = 0; i < 10; i++) {
                    selected[i] += omp_get_thread_num();
                    printf("Selected + thread: %d\n", selected[i]);
                }
            }
        }
    }
}*/

void operate() {
    #pragma omp parallel
    {
        #pragma omp single
        {
            #pragma omp task
            {
                #pragma omp task
                {
                    for (int i = 0; i < N; i++) {
                        sum += i;
                        if (sum % 8 == 0) {
                            printf("Sum %d\n", sum);
                        }
                    }
                }

                #pragma omp critical
                {
                    int selected[10] = {0};
                    for (int i = 0; i < 10; i++) {
                        int ind = rand() % N;
                        selected[i] = nums[ind];
                    }
                    #pragma omp task
                    {
                        for (int i = 0; i < 10; i++) {
                            selected[i] += omp_get_thread_num();
                            printf("Selected + thread: %d\n", selected[i]);
                        }
                    }
                }
            }
        }
    }
}

void main() {
    for (int i = 0; i < N; i++) {
        nums[i] = i + 1;
    }
    operate();
}