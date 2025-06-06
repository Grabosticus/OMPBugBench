#include <stdio.h>
#include <omp.h>
#include <unistd.h>

#define N_TH 2
#define N_TR 5

void main() {
    int A = 100;
    int B = 200;

    omp_lock_t lckA, lckB;
    omp_init_lock(&lckA);
    omp_init_lock(&lckB);

    omp_set_num_threads(N_TH);

    #pragma omp parallel shared(A, B, lckA, lckB)
    {
        int tid = omp_get_thread_num();

        for (int i = 0; i < N_TR; i++) {

            if (tid == 0) {
                omp_set_lock(&lckA);
                
                A += 10;

                omp_set_lock(&lckB);

                B += A * 2;

                omp_unset_lock(&lckB);
                omp_unset_lock(&lckA);

            } else if (tid == 1) {
                omp_set_lock(&lckB);

                omp_set_lock(&lckA);

                B -= 20;
                A += 20;

                omp_unset_lock(&lckA);
                omp_unset_lock(&lckB);
            }
        }
    }

    omp_destroy_lock(&lckA);
    omp_destroy_lock(&lckB);

    printf("A: %d\n", A);
    printf("B: %d\n", B);
}