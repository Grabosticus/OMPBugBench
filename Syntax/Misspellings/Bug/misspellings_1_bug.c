#include <stdio.h>
#include <omp.h>

double array_sum(double *summand1, double *summand2, int length)
{
   int i;

   double tmp, sum;

   sum = 0.0;

   pragma omp sind private(tmp) reduction(+:sum)
   for (i = 0; i < length; i++) {

      tmp = summand1[i] + summand2[i];

      sum += tmp;

   }

   return sum;
}

void main() {
    double summand1[3] = {1, 5, 8};

    double summand2[3] = {2.23, 5.2832, 9.9129};

    int length = 3;

    printf("%f\n", array_sum(summand1, summand2, length));
}