/*
Demonstrates device mapping with OpenMP
Bug Line: 10
Difficulty: Easy
*/

#include <stdio.h>
#include <omp.h>

void map_()
{
    int arr[30], *p;
    #pragma omp target data map(arr[0:4] )
    {
        p = &arr[0];
        #pragma omp target map(p[7:20] )
        {
            arr[2] = 0;
            p[8] = 0;
        }
    }
}

void main() {
    map_();
}