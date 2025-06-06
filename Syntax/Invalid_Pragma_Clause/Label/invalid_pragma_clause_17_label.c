/*
Demonstrates vector multiplication using OpenMP device offloading.
Bug Lines: 12 (added curly braces after if(do_offload), wrote "to" and "from" in same argument, invalid range for "from"),
13 (added curly braces after if statement)
Difficulty: Easy
*/

#include <omp.h>
#include <stdio.h>

void vec_mult(float p[], float v1[], float v2[], int N)
{
   int i;

   int device_number = omp_get_num_devices();

   int do_offload = (device_number>0 && N>1000000);

   #pragma omp target if(do_offload) map(to: v1[0:N], v2[:N]) map(from: p[0:N])
   #pragma omp parallel for if(N>1000) private(i)
   for (i=0; i<N; i++)
     p[i] = v1[i] * v2[i];
}

void main() {
    float p[3] = {0, 0, 0};
    float v1[3] = {4, 5, 6};
    float v2[3] = {7, 9, 9};
    int N = 3;
    vec_mult(p, v1, v2, N);
    printf("%f %f %f\n", p[0], p[1], p[2]);
}