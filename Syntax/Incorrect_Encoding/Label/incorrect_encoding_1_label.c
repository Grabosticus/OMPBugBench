/*
Replaces values in a array between indices m and (n-1) with values at other array positions.
Bug Line: 7 (replaced "a" with cyrillic a and "p" with cyrrilic p)
Difficulty: Easy
*/

#include <stdio.h>
#include <omp.h>

void replace(float *arr, int high, int low)
{
   int i;
   #pragma omp simd safelen(16)
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