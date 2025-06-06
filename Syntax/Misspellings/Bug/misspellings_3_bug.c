#include <stdio.h>
#include <omp.h>

void work(int i, float *a) {
    a[0] = i*2;
}

int next(float *a) {
    return a[0];
}

void compute(float *arr1, float *arr2)
{
  int ix_next, iy_next;

  #pragm OMP paralel shared{arr1, arr2} private{ix_next, iy_next}
  {
    #pragm OMP crit (xaxis)
      ix_next = next(arr1);
    work(ix_next, arr1);

    #pragm OMP crit (yaxis)
      iy_next = next(arr2);
    work(iy_next, arr2);
  }

}

void main() {
    float arr1[1] = {5};
    float arr2[1] = {9};
    compute(arr1, arr2);
    printf("arr1: %f\n", arr1[0]);
    printf("arr2: %f\n", arr2[0]);
}