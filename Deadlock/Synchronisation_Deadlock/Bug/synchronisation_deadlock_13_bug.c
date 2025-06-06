#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>

omp_lock_t print_lock;

void find(int n) {
      if (n < 2) {
        return;
    }

    char *is_prime = calloc(n, sizeof(char));
    for (int i = 2; i < n; ++i)
        is_prime[i] = 1;

    #pragma omp parallel
    {
        #pragma omp single
        {
            for (int i = 2; i < sqrt(n); ++i) {
                if (is_prime[i]) {
                    #pragma omp task firstprivate(i)
                    {
                        for (int j = i * i; j < n; j += i) {
                            is_prime[j] = 0;
                        }
                    }
                }
            }
        }
    }

    omp_set_lock(&print_lock);
    printf("Primes below %d:\n", n);
    for (int i = 2; i < n; ++i) {
        if (is_prime[i])
            printf("%d ", i);
    }
    printf("\n");
    omp_unset_lock(&print_lock);

    free(is_prime);
}

void find_primes(int n)
{
  find(n);
  #pragma omp barrier
}

void paral(int n)
{
  int i;
  #pragma omp parallel private(i) shared(n)
  {
    #pragma omp for
    for (i=0; i<n*5; i+=5)
      find_primes(i);
  }
}

int main()
{
  omp_init_lock(&print_lock);
  paral(10);
  omp_destroy_lock(&print_lock);
  return 0;
}