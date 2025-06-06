#include <stdio.h>
#include <omp.h>

#define N 100
omp_lock_t l1, l2;
int happy[N], weather[N];

void operate() {
    int nthreads = 2;
    omp_set_num_threads(nthreads);
    #pragma omp parallel shared(weather, happy, nthreads, l1, l2)
    {
        #pragma omp sections nowait
        {
            #pragma omp section
            {
                omp_set_lock(&l1);
                for (int i=0; i<N; i++) {
                    weather[i] = i * 9;
                }

                omp_set_lock(&l2);
                for (int i=0; i<N; i++) {
                    happy[i] = 5*weather[i];
                }
                omp_unset_lock(&l2);
                omp_unset_lock(&l1);
            }

            #pragma omp section
            {
                omp_set_lock(&l2);
                for (int i=0; i<N; i++) {
                    happy[i] = i * 2;
                }

                omp_set_lock(&l1);
                for (int i=0; i<N; i++) {
                    weather[i] = 2*happy[i] + 1;
                }
                omp_unset_lock(&l1);
                omp_unset_lock(&l2);
            }
        }
    }
}

void main() {
    omp_init_lock(&l1);
    omp_init_lock(&l2);
    operate();
    omp_destroy_lock(&l1);
    omp_destroy_lock(&l2);
}