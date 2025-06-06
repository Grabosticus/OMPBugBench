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
                #pragma omp critical(B)
                {
                    #pragma omp critical(A)
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