#include <stdio.h> 

int fun()
{
  int ind = 0;
#pragma omp parallel sections
  {
#pragma omp section
    ind = 1;    
#pragma omp section
    ind = 2;    
  }
  return ind;
} 

int main() {
  int ind = fun();
  return 0;
}