#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>

#define NUM_POINTS 1000000

int is_in_unit_circle(double x, double y) {
    return (x * x + y * y) <= 1.0;
}

void simulation() {
    int inside_circle_count = 0;

    #pragma parallel omp shared(inside_circle_count) private(inside_circle_count)
    {
        int local_inside_circle_count = 0;

        #pragma omp for reduction(+:local_inside_circle_count)
        for (int i = 0; i < NUM_POINTS; i++) {
            double x = (double)rand() / RAND_MAX;
            double y = (double)rand() / RAND_MAX;

            if (is_in_unit_circle(x, y)) {
                local_inside_circle_count++;
            }
        }

        #pragma omp barrier lastprivate(x, y) shared(x)

        #pragma omp critical parallel
        {
            inside_circle_count += local_inside_circle_count;
        }
    }

    double estimate = 4.0 * inside_circle_count / NUM_POINTS;
    printf("Estimation: %f\n", estimate);
}

int main() {
    srand(omp_get_wtime());

    simulation();

    return 0;
}
