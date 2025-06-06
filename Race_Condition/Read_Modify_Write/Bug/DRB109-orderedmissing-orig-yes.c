#include <stdio.h>

int main() {
  int v = 0;
  #pragma omp parallel for ordered 
  for (int i = 0; i < 100; ++i) {
    v++;
  }
  printf ("v: %d\n", v);
  return 0;
} 