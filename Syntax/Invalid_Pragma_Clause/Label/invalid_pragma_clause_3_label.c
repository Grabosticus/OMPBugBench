/*
Offloads the dot product calculation to the target device with id 30
Bug Lines: 10, 12 ("target" keyword missing), 16 ("omp" keyword missing), 35 ("omp" keyword missing)
Difficulty: Medium
*/

#include <stdio.h>
#include <omp.h>

void print_out(float *v, int N) {
    for (int i = 0; i < N; i++) {
        printf("%f\n", v[i]);
    }
}

#pragma omp declare target
void dot_product(float *dot, float *vector1, float *vector2, int N);
#pragma omp end declare target

void map_device(float *v, float *vector1, float *vector2, int N)
{
   #pragma omp target device(30) map(v[:N], vector1[:N], vector2[:N])
   {
      dot_product(v, vector1, vector2, N);
   }
   print_out(v, N);
}

void dot_product(float *dot, float *vector1, float *vector2, int N)
{
   int i;
   int nthreads;

   if (!omp_is_initial_device()) {
      nthreads = 1024;
      printf("Running in target device\n");
   } else {
      nthreads = 8;
   }

   #pragma omp parallel for private(i) num_threads(nthreads)
   for (i=0; i<N; i++)
     dot[i] = vector1[i] * vector2[i];
}

void main() {
    float vector1[3] = {1, 4, 7};
    float vector2[3] = {2, 5, 8};
    int N = 3;
    float dot[3];
    map_device(dot, vector1, vector2, N);
}