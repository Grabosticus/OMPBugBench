/*
Simulates communication between two threads using OpenMP.
Bug: Thread 0 and Thread 1 set f_A/f_B only after waiting for the message from the other thread, leading to a deadlock.
Difficulty: Easy
*/

#include <stdio.h>
#include <omp.h>

int main() {
    int m_A = 0;
    int m_B = 0;
    int f_A = 0;
    int f_B = 0;

    omp_lock_t l_A;
    omp_lock_t l_B;
    omp_init_lock(&l_A);
    omp_init_lock(&l_B);

    omp_set_num_threads(2);

    #pragma omp parallel shared(m_A, m_B, f_A, f_B, l_A, l_B)
    {
        int tid = omp_get_thread_num();

        if (tid == 0) { 
            omp_set_lock(&l_A);
            m_A = 100;
            printf("A: Sent %d\n", m_A);
            #pragma omp flush(m_A)
            f_A = 1;
            #pragma omp flush(f_A)

            while (!f_B) {
                #pragma omp flush(f_B)
            }
            #pragma omp flush(m_B)
            printf("A: Received %d\n", m_B);

            omp_unset_lock(&l_A);
        }

        if (tid == 1) {
            omp_set_lock(&l_B);
            m_B = 200;
            #pragma omp flush(m_B)
            printf("B: Sent %d\n", m_B);
            f_B = 1;
            #pragma omp flush(f_B)

            while (!f_A) {
                #pragma omp flush(f_A)
            }
            #pragma omp flush(m_A)
            printf("B: Received %d\n", m_A);

            omp_unset_lock(&l_B);
        }
    }

    omp_destroy_lock(&l_A);
    omp_destroy_lock(&l_B);

    return 0;
}
