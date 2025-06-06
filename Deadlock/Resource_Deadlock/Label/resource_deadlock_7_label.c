/*
Demonstrates the use of OpenMP sections.
Bug: Thread 0 holds the lock for critical region A and wants to enter B. However thread 1 holds the lock for B and wants to enter A --> Deadlock.
Difficulty: Easy
*/

#include <stdio.h>
#include <omp.h>

#define N 10

int shared_arrA[N] = {0};
int shared_arrB[N] = {0};

void operate() {
    #pragma omp parallel
    {
        #pragma omp sections
        {
            #pragma omp section
            {
                #pragma omp critical(A)
                {
                    for (int i = 0; i < N; i++) {
                        shared_arrA[i] += i;
                    }

                    #pragma omp critical(B)
                    {
                        for (int i = 0; i < N; i++) {
                            shared_arrB[i] += shared_arrA[i] * i;
                        }
                    }
                }
            }

            #pragma omp section
            {
                #pragma omp critical(A)
                {
                    #pragma omp critical(B)
                    {
                        for (int i = 0; i < N; i++) {
                            printf("%d %d\n", shared_arrA[i], shared_arrB[i]);
                        }
                    }
                }
            }
        }
    }
}

void main() {
    operate();
}