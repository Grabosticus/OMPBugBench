#include <stdio.h>
#include <omp.h>

omp_lock_t print_lock;

int sum_of_digits_depth(int n, int depth, int print) {
    if (n < 10)
        return n;

    int digit = n % 10;
    int rest = n / 10;
    int part1 = 0, part2 = 0;

    if (print) {
        omp_set_lock(&print_lock);
        printf("n: %d, depth: %d\n", n, depth);
    }

    #pragma omp task shared(part1)
    {
        if (depth < 4) {
            part1 = sum_of_digits_depth(rest, depth + 1, print);
        } else {
            part1 = rest % 10;
        }
    }

    #pragma omp task shared(part2)
    {
        part2 = digit;
        if (print) {
            omp_set_lock(&print_lock);
            printf("digit: %d\n", digit);
            omp_unset_lock(&print_lock);
        }
    }

    #pragma omp taskwait

    if (print) {
        printf("%d + %d = %d\n", part1, part2, (part1 + part2));
        omp_unset_lock(&print_lock);
    }

    return part1 + part2;
}

int main() {
    int num = 987654321;
    int result = 0;

    omp_init_lock(&print_lock);

    #pragma omp parallel
    {
        #pragma omp single
        {
            result = sum_of_digits_depth(num, 0, 1);
        }
    }

    omp_destroy_lock(&print_lock);
    printf("Sum of %d up to depth 4: %d\n", num, result);
    return 0;
}