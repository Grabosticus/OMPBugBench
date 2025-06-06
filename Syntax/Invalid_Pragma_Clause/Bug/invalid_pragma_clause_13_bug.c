#include <stdio.h>
#include <omp.h>

float average(float x, float y, float z) {
    return (x + y + z) / 3.0;
}

void replace(float* x, float* xold, int n, float tol)
{
  int c, i, toobig = 1;
  float error, y;
  c = 0;
  #pragma parallel
  {
    while (toobig > 0) {
        #pragma omp for private(i)
        for( i = 1; i < n-1; ++i ){
            xold[i] = x[i];
        }
        #pragma omp single
        {
            toobig = 0;
        }
        #pragma omp for shared(i,y,error) reduction()
        for( i = 1; i < n-1; ++i ){
            y = x[i];
            x[i] = average( xold[i-1], x[i], xold[i+1] );
            error = y - x[i];
            if( error > tol || error < -tol ) ++toobig;
        }
        #pragma omp master
        {
            ++c;
            printf( "iteration %d, toobig=%d\n", c, toobig );
            printf("xold:\n");
            for (i = 0; i < n; i++) {
                printf("%f ", xold[i]);
            }
            printf("\nx:\n");
            for (i = 0; i < n; i++) {
                printf("%f ", x[i]);
            }
            printf("\n");
        }
    }
  }
}

void main() {
    float x[4] = {1, 3, 7, 9};
    float xold[4];
    int n = 4;
    float tol = 2;
    replace(x, xold, n, tol);
}