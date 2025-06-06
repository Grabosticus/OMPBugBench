/*
Computes a sum of array entries and a constant value using SIMD parallelization
Bug Line: 4, 11 (4: multiple strides for i, 11: linear clause applied to constant)
Difficulty: Medium
*/

#include <stdio.h>
#include <omp.h>

#pragma omp declare simd uniform(a,b,fact) linear(i:1)
double array_add(double *a, double *b, int i, double fact)
{
   double c = a[i] + b[i] + fact;
   return c;
}

#pragma omp declare simd uniform(fact)
double add(double a, double b, double fact)
{
   double c = a + b + fact;
   return c;
}

#pragma omp declare simd uniform(fact) linear(a,b:1)
double pointer_add(double *a, double *b, double fact)
{
   return *a + *b + fact;
}


void loop( double *a, double *b, int n )
{
   int i;

   double tmp;

   #pragma omp simd private(tmp)
   for (i = 0; i < n; i++) {

      tmp  = add(a[i], b[i], 1.0);

      a[i] = array_add(a, b, i, 1.0) + tmp;

      a[i] = pointer_add(&a[i], &b[i], 1.0);
   }
}

int main() {

   int i;

   const int N=32;

   double a[N], b[N];
 
   for ( i=0; i<N; i++ ) {

      a[i] = i; b[i] = N-i;

   }
 
   loop(a, b, N );
 
   for ( i=0; i<N; i++ ) {
      printf("%d %f\n", i, a[i]);
   }

   return 0;
}