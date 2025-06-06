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
    #pragma omp task // Task 1
    {
        #pragma omp task // Task 2
        {
            int local_sum = 0;
            for (int i = 0; i < N; i++) {
                local_sum += nums[i];
                if (local_sum % 8 == 0) {
                    printf("Partial Local Sum %d\n", local_sum);
                }
            }
            #pragma omp critical // Critical Region 1
            sum += local_sum;
        }

        #pragma omp critical // Critical Region 2
        {
            int selected[10] = {0};
            int max_val = -1;
            int min_val = N + 1;
            double avg = 0.0;

            for (int i = 0; i < 10; i++) {
                int ind = rand() % N;
                selected[i] = nums[ind];
                if (selected[i] > max_val) max_val = selected[i];
                if (selected[i] < min_val) min_val = selected[i];
                avg += selected[i];
            }

            avg /= 10.0;
            printf("Selected Stats -> Max: %d, Min: %d, Avg: %.2f\n", max_val, min_val, avg);

            #pragma omp task // Task 3
            {
                for (int i = 0; i < 10; i++) {
                    selected[i] += omp_get_thread_num();
                    printf("Selected + thread: %d\n", selected[i]);
                }

                int square_sum = 0;
                for (int i = 0; i < 10; i++) {
                    square_sum += selected[i] * selected[i];
                }
                printf("Sum of squares: %d\n", square_sum);
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
                    int local_sum = 0;
                    for (int i = 0; i < N; i++) {
                        local_sum += nums[i];
                        if (local_sum % 8 == 0) {
                            printf("Partial Local Sum %d\n", local_sum);
                        }
                    }
                    sum += local_sum;
                }

                #pragma omp critical
                {
                    int selected[10] = {0};
                    int max_val = -1;
                    int min_val = N + 1;
                    double avg = 0.0;

                    for (int i = 0; i < 10; i++) {
                        int ind = rand() % N;
                        selected[i] = nums[ind];
                        if (selected[i] > max_val) max_val = selected[i];
                        if (selected[i] < min_val) min_val = selected[i];
                        avg += selected[i];
                    }

                    avg /= 10.0;
                    printf("Selected Stats -> Max: %d, Min: %d, Avg: %.2f\n", max_val, min_val, avg);

                    #pragma omp task
                    {
                        for (int i = 0; i < 10; i++) {
                            selected[i] += omp_get_thread_num();
                            printf("Selected + thread: %d\n", selected[i]);
                        }

                        int square_sum = 0;
                        for (int i = 0; i < 10; i++) {
                            square_sum += selected[i] * selected[i];
                        }
                        printf("Sum of squares: %d\n", square_sum);
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