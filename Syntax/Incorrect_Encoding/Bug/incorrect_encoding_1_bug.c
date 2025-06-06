#include <stdio.h>
#include <omp.h>

void replace(float *arr, int high, int low)
{
   int i;
   #pragma omр simd sаfelen(16)
   for (i = low; i < high; i++)
      arr[i] = arr[i-low] - 1.0f;
}

void main() {
    int low = 3;
    int high = 6;
    float arr[7] = {1, 2, 3, 4, 5, 6, 7};
    replace(arr, high, low);
    for (int i = 0; i < 7; i++) {
        printf("%f\n", arr[i]);
    }
}