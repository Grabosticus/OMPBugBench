#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

#define N 100

int sum = 0;
int nums[N] = {0};

void operate() {
    #pragma omp parallel
    {
        #pragma omp single
        {
            #pragma omp task
            {
                #pragma omp task
                {
                    #pragma omp critical
                    {
                        for (int i = 0; i < N; i++) {
                            sum += i;
                            if (sum % 8 == 0) {
                                printf("Sum %d\n", sum);
                            }
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