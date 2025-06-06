#include <stdio.h>
#include <omp.h>
#include <unistd.h>

omp_lock_t lockX, lockY, lockZ;
int X = 1, Y = 2, Z = 3;

void transferZ() {
    omp_set_lock(&lockZ);
    omp_set_lock(&lockX);
    Z += X;
    omp_unset_lock(&lockX);
    omp_unset_lock(&lockZ);
}

void transferXY() {
    omp_set_lock(&lockX);
    X += 1;
    omp_set_lock(&lockY);
    Y += 2;
    omp_unset_lock(&lockY);
    omp_unset_lock(&lockX);
}

void transferXZ() {
    omp_set_lock(&lockY);
    omp_set_lock(&lockZ);
    X += Z + 1;
    Z += X - 1;
    omp_unset_lock(&lockZ);
    omp_unset_lock(&lockY);
}

int main() {
    omp_init_lock(&lockX);
    omp_init_lock(&lockY);
    omp_init_lock(&lockZ);

    omp_set_num_threads(3);

    #pragma omp parallel
    {
        int tid = omp_get_thread_num();

        if (tid == 0)
            transferZ();
        else if (tid == 1)
            transferXY();
        else if (tid == 2)
            transferXZ();
    }

    printf("X: %d\n", X);
    printf("Y: %d\n", Y);
    printf("Z: %d\n", Z);

    omp_destroy_lock(&lockX);
    omp_destroy_lock(&lockY);
    omp_destroy_lock(&lockZ);

    return 0;
}