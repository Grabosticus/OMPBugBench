/*
Demonstrates device mapping with OpenMP.
Bug Lines: 5, 7, 11, 12
Difficulty: Easy
*/

#include <omp.h>

void mapping() {
    int arr[30], *pointer;
    #pragma omp target data map( arr[0:10] )
    {
        pointer = &arr[0];
        #pragma omp target map( pointer[3:7] )
        {
            arr[2] = 0;
            pointer[8] = 0;
            arr[8] = 1;
        }
    }
}

void main() {
    mapping();
}