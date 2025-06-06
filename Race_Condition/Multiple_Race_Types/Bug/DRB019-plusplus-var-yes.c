#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
  int i;
  int input = 1000; 
  int output = 0;

  if (argc > 1)
    input = atoi(argv[1]);

  int input_arr[input]; 
  int output_arr[input];

  for (i=0; i < input; ++i) 
    input_arr[i]=i; 

#pragma omp parallel for
  for (i=0; i<input; ++i) {
    output_arr[output++] = input_arr[i] ;
  }  

  printf("output_arr[0]: %d\n", output_arr[0]);
  return 0;
}