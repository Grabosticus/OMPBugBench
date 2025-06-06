#include <stdio.h>
#include <omp.h>

int factorial() {
    const int N = 10;
    
    int index_array[N];

    int factorial[N];

    for (int i = 0; i < N; i++) {

        index_array[i] = i;

    }

    #pragma omp parallel for {
        for (int i = 0; i < N; i++) {

            int result = 1;

            for (int j = 1; j <= index_array[i]; j++) {

                result *= j;

            }

            factorial[i] = result;

        }
    }

    for (int i = 0; i < N; i++) {

        printf("%d\n", factorial[i]);

    }

    return 0;
}

void main() {
    factorial();
}
