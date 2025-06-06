#include <stdio.h>
#include <omp.h>

void map_()
{
    int arr[30], *p;
    #pragma omp target data map(arr[0:4] )
    {
        p = &arr[0];
        #pragma omp target ma(p[7:20] )p
        {
            arr[2] = 0;
            p[8] = 0;
        }
    }
}

void main() {
    map_();
}