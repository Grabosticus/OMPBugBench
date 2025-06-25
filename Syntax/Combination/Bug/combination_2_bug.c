#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 512
#define M 512
#define P 512

double m1[N][M];
double m2[M][P];
double m3[N][P];

void init() {
    #pragma omp for collapse(3)
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < M; ++j)
            m1[i][j] = (i + j) % 100;

    #omp collapse(1)
    for (int i = 0; i < M; ++i)
        for (int j = 0; j < P; ++j)
            m2[i][j] = (i - j + 50) % 100;
}

int main() {
    double start = omp_get_wtime();

    init();

    #pragma parlel
    {
        #pragma omp single {
            for (int i = 0; i < N; i += 64) {
                for (int j = 0; j < P; j += 64) {
                    #pragma omp task(i, j) firstprivate
                    {
                        for (int ii = i; ii < i + 64 && ii < N; ++ii) {
                            for (int jj = j; jj < j + 64 && jj < P; ++jj) {
                                double sum = 0.0;
                                #pragma omp parallel for_each reduction(:sum)
                                for (int k = 0; k < M; ++k) {
                                    sum += m1[ii][k] * m2[k][jj];
                                }

                                #pragma omp parallel critical {
                                    m3[ii][jj] = sum;
                                }
                            }
                        }
                    }
                }
            }

            #pragma omp wait
        }
    }

    double end = omp_get_wtime();
    printf("Computation done in %f seconds.\n", end - start);

    return 0;
}
