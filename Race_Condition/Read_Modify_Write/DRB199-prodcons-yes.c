/*
!!!~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~!!!
!!! Copyright (c) 2017-20, Lawrence Livermore National Security, LLC
!!! and DataRaceBench project contributors. See the DataRaceBench/COPYRIGHT file for details.
!!!
!!! SPDX-License-Identifier: (BSD-3-Clause)
!!!~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~!!!
 */ // DIFFICULTY: Medium

/*
 * This is a program based on a dataset contributed by
 * Wenhao Wu and Stephen F. Siegel @Univ. of Delaware.

 * race introduced because critical sections have different names for producer and consumer.
 * Data race pair: size@34:11:W vs. size@45:11:W
 */

#include <stdio.h>
int nprod = 4, ncons = 4;
int cap = 5, size = 0, packages = 1000;
int main()
{
  int nthread = nprod + ncons;
#pragma omp parallel for shared(size, cap, nprod, ncons, nthread) firstprivate(packages) num_threads(nthread)
  for (int i = 0; i < nthread; i++)
  {
    if (i < nprod)
      while (packages)
      { // I am a producer
#pragma omp critical(A)
        if (size < cap)
        {
          size++; // produce
          packages--; // produced a package
          printf("Producer %d produced! size=%d\n", i, size);
          fflush(stdout);
        }
      }
    else
      while (packages)
      { // I am a consumer
#pragma omp critical(B)
        if (size > 0)
        {
          size--; // consume
          packages--; // consumed a package
          printf("Consumer %d consumed! size=%d\n", i - nprod, size);
          fflush(stdout);
        }
      }
  }
}