/*
Computes a matrix multiplication
Bug Lines:
14 ("collapse()"-argument too high)
19 ("pragma" missing, collapse is set to "1", even though 2 nested loops are used)
30 ("omp" missing, "parallel" misspelled)
32 (curly brace written at the end of pragma statement, instead of next line)
35 ("task" got the arguments of "firstprivate()")
40 (reduction operation argument missing, "for" written as "for_each")
45 ("parallel"-keyword added to "critical"-directive, curly brace is placed wrong)
54 ("taskwait" written as "wait")
Difficulty: Hard
*/

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
    #pragma omp for collapse(2)
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < M; ++j)
            m1[i][j] = (i + j) % 100;

    #pragma omp for collapse(2)
    for (int i = 0; i < M; ++i)
        for (int j = 0; j < P; ++j)
            m2[i][j] = (i - j + 50) % 100;
}

int main() {
    double start = omp_get_wtime();

    init();

    #pragma omp parallel
    {
        #pragma omp single
        {
            for (int i = 0; i < N; i += 64) {
                for (int j = 0; j < P; j += 64) {
                    #pragma omp task firstprivate(i, j)
                    {
                        for (int ii = i; ii < i + 64 && ii < N; ++ii) {
                            for (int jj = j; jj < j + 64 && jj < P; ++jj) {
                                double sum = 0.0;
                                #pragma omp parallel for reduction(+:sum)
                                for (int k = 0; k < M; ++k) {
                                    sum += m1[ii][k] * m2[k][jj];
                                }

                                #pragma omp critical
                                {
                                    m3[ii][jj] = sum;
                                }
                            }
                        }
                    }
                }
            }

            #pragma omp taskwait
        }
    }

    double end = omp_get_wtime();
    printf("Computation done in %f seconds.\n", end - start);

    return 0;
}
