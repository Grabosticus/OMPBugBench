/*
Increases a counter.
Bug: The lock is set double. One time in the main() for loop and the other time in the increment() function, leading to the thread deadlocking itself.
Difficulty: Easy
*/

#include <stdio.h>
#include <omp.h>


omp_lock_t lock;

int increment(int counter) {
    counter = counter + 1;
    return counter;
}

void print_it(int tid, int counter) {
    printf("Thread %d: Counter %d\n", tid, counter);
}

int main() {
    int counter = 0;

    omp_init_lock(&lock);

    #pragma omp parallel num_threads(4)
    {
        int tid = omp_get_thread_num();
        for (int i = 0; i < 5; i++) {
            omp_set_lock(&lock);

            print_it(tid, counter);
            counter = increment(counter);

            omp_unset_lock(&lock);
        }
    }

    omp_destroy_lock(&lock);

    printf("Final counter value: %d\n", counter);
    return 0;
}
