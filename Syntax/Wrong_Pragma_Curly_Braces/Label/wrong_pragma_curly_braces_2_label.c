/*
Computes the dot product of vector1 and vector2
Bug Lines: 24 and 33
Difficulty: Easy
*/

#include <stdio.h>
#include <omp.h>

void initialize_vectors(float* vector1, float* vector2, int N) {

    for (int i = 0; i < N; i++) {
        vector2[i] = i/2;
        vector1[i] = i*2;
    }

}

void dot_product(int N)
{

   int i;

   float dot[N], vector1[N], vector2[N];

   initialize_vectors(vector1, vector2, N);

   #pragma omp target map(vector1, vector2, dot)
   #pragma omp parallel for
   for (i=0; i<N; i++)
     dot[i] = vector1[i] * vector2[i];

    for (i = 0; i < N; i++) {

        printf("%f\n", dot[i]);

    }
}

void main() {
    dot_product(10);
}