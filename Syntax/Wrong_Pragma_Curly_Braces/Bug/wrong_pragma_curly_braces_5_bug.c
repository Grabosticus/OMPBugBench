#include <omp.h>

void mapping ()
{
   int array[30], *pointer;
#pragma omp target data map( array[0:10] ) {

   pointer = &array[0];

   #pragma omp target map( pointer[3:7] )
   {
      array[2] = 0;

      pointer[8] = 0;

      array[8] = 1;
   }

}

}

void main() {
    mapping();
}