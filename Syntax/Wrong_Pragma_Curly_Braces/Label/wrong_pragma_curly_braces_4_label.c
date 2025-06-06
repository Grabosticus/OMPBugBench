/*
Prints out the thread number and iteration id. Because of "ordered" it will always be the same
Bug Lines: 8 and 19
Difficulty: Easy
*/

#include <omp.h>
#include <stdio.h>

void print_ordered()
{

   int id_2, id_1;
   #pragma omp parallel num_threads(2)
   {

      #pragma omp for collapse(2) ordered private(id_2,id_1) schedule(static,3)
      for (id_1=1; id_1<=3; id_1++)
         for (id_2=1; id_2<=2; id_2++)
         {

            #pragma omp ordered
            printf("%d %d %d\n", omp_get_thread_num(), id_1, id_2);

         }
   }

}

void main() {
    print_ordered();
}