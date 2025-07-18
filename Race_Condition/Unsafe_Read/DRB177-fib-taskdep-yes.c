/*
!!!~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~!!!
!!! Copyright (c) 2017-20, Lawrence Livermore National Security, LLC
!!! and DataRaceBench project contributors. See the DataRaceBench/COPYRIGHT file
for details.
!!!
!!! SPDX-License-Identifier: (BSD-3-Clause)
!!!~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~!!!
 */ // Difficulty: Medium; changed the bug code to return s instead of "first + second" to better showcase the race

/* 
 * Fibonacci code with data race (possible to scale problem size by providing
 * size argument).
 * Data Race Pair, i@25:5:W vs. i@29:7:R
 * */

#include <stdio.h>
#include <stdlib.h>

int fib(int n) {
  int i, j, s;
  if (n < 2)
    return n;
#pragma omp task shared(i) depend(out : i)
  i = fib(n - 1);
#pragma omp task shared(j) depend(out : j)
  j = fib(n - 2);
#pragma omp task shared(i, j) depend(in : j)
  s = i + j;
#pragma omp taskwait
  return i + j;
}

int main(int argc, char **argv) {
  int n = 10;
  if (argc > 1)
    n = atoi(argv[1]);
#pragma omp parallel sections
  { printf("fib(%i) = %i\n", n, fib(n)); }
  return 0;
}