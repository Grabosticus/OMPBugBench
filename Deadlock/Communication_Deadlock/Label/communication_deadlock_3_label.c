/*
Two Producers write a value, a Processor then reads them and performs an operation on 
them, and a Consumer performs an operation on the result and prints it out.
Bug: The Producers wait for the variable "m" to be set to 1, before conducting their operations. However, "m" is only set to 1
in the Consumer, after the Processor, which needs the values from the Producers, did their operations. --> Deadlock
Difficulty: Medium
*/

#include <stdio.h>
#include <omp.h>

int main() {
    int a = 0, b = 0;
    int c = 0;
    int d = 0;

    int e = 0, f = 0;
    int g = 0;

    omp_lock_t h, i, j;

    omp_init_lock(&h);
    omp_init_lock(&i);
    omp_init_lock(&j);

    omp_set_num_threads(4);

    #pragma omp parallel shared(a, b, c, d, e, f, g, h, i, j)
    {
        int k = omp_get_thread_num();

        if (k == 0) {
            int l = 15;
            omp_set_lock(&h);
            a = l;
            e = 1;
            printf("P 0: %d\n", l);
            #pragma omp flush(a, e)
            omp_unset_lock(&h);
        }

        if (k == 1) {
            int l = 8;
            omp_set_lock(&i);
            b = l;
            f = 1;
            printf("P 1: %d\n", l);
            #pragma omp flush(b, f)
            omp_unset_lock(&i);
        }

        if (k == 2) {
            while (!(e && f)) {
                #pragma omp flush(e, f)
            }

            omp_set_lock(&h);
            omp_set_lock(&i);
            #pragma omp flush(a, b)

            c = a + b;
            g = 1;
            printf("%d + %d = %d\n", a, b, c);

            #pragma omp flush(c, g)
            omp_unset_lock(&h);
            omp_unset_lock(&i);
        }

        if (k == 3) {
            while (!g) {
                #pragma omp flush(g)
            }

            omp_set_lock(&j);
            #pragma omp flush(c)

            d = c * 5;
            printf("C: result %d\n", d);

            omp_unset_lock(&j);
        }
    }

    omp_destroy_lock(&h);
    omp_destroy_lock(&i);
    omp_destroy_lock(&j);

    return 0;
}

