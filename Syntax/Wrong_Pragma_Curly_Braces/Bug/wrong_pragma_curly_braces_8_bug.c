#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 16

void _init(int *array, int n) {
    #pragma omp parallel for {
        for (int i = 0; i < n; i++) {
            array[i] = i + 1;
        }
    }
}

void _factorials(int *input, int *output, int n) {
    #pragma omp parallel for schedule(dynamic) {
        for (int i = 0; i < n; i++) {
            int fact = 1;
            for (int j = 2; j <= input[i]; j++) {
                fact *= j;
            }
            output[i] = fact;
        }
    }
}

void _prefix_sum(int *input, int *output, int n) {
    output[0] = input[0];

    #pragma omp parallel
    {
        #pragma omp single {
            for (int i = 1; i < n; i++) {
                #pragma omp task firstprivate(i) {
                    output[i] = output[i-1] + input[i];
                }
            }
        }
        #pragma omp taskwait
    }
}

void section_processing(int *array, int n) {
    #pragma omp parallel sections {
        #pragma omp section {
            #pragma omp critical
            printf("Section 1: First element is %d\n", array[0]);
        }

        #pragma omp section {
            int sum = 0;
            #pragma omp parallel for reduction(+:sum) {
                for (int i = 0; i < n; i++) {
                    sum += array[i];
                }
            }
            #pragma omp critical
            printf("Section 2: Sum is %d\n", sum);
        }
    }
}

int main() {
    int input[N], factorials[N], prefix[N];

    _init(input, N);

    _factorials(input, factorials, N);

    #pragma omp target data map(to: factorials[0:N]) map(from: prefix[0:N]) {
        #pragma omp target teams distribute parallel for {
            for (int i = 0; i < N; i++) {
                prefix[i] = factorials[i] % 100;
            }
        }
    }

    _prefix_sum(input, prefix, N);

    section_processing(prefix, N);

    #pragma omp barrier

    printf("Final results:\n");
    for (int i = 0; i < N; i++) {
        printf("prefix[%d] = %d\n", i, prefix[i]);
    }

    return 0;
}