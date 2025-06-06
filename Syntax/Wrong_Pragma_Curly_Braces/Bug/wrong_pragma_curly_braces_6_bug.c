#include <stdio.h>
#include <omp.h>

#define N 1000

void initialize_arrays(int n, float *arr1, float *arr2) {
    for (int i = 0; i < n; i++) {
        arr1[i] = i;
        arr2[i] = 0. - i;
    }
}

int main() {
   int i, n = N;
   int chunk = 1000;
   float arr1[N];
   float arr2[N];
   float res[N];

   initialize_arrays(n, arr1, arr2);

   #pragma omp parallel
   {

      #pragma omp master {
        #pragma omp target teams distribute parallel for nowait map(to: arr1[0:n/2]) map(to: arr2[0:n/2]) map(from: res[0:n/2]) {
            for(i=0; i<n/2; i++){ res[i] = arr1[i]*arr2[i]; }

            #pragma omp for schedule(dynamic,chunk) {
                for(i=n/2; i<n; i++){ res[i] = arr1[i]*arr2[i]; }
            }
        }
      }
   }
   printf(" res[0] res[n-1] %f %f\n", res[0], res[n-1]);
   return 0;
}