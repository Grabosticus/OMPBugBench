/*
Processes multiple tasks using threads.
Data Race Lines: 34 vs. 36
Difficulty: Medium
*/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define TASKS 5
#define THREADS 4

int tasks[TASKS];

void process_task(int task_id, int thread_id) {
    int sum = 0;
    for (volatile int i = 0; i < 10000000; ++i) {
        sum += task_id + thread_id + i;
    }
    printf("Sum of task %d: %d\n", task_id, sum);
}

int main() {
    for (int i = 0; i < TASKS; i++) {
        tasks[i] = 0;
    }

#pragma omp parallel num_threads(THREADS) shared(tasks)
    {
        int tid = omp_get_thread_num();

        for (int i = 0; i < TASKS; i++) {
            if (tasks[i] == 0) {
                printf("Thread %d: Processing task %d\n", tid, i);
                tasks[i] = 1;
                process_task(i, tid);
            }
        }
    }

    printf("\nFinal task processing states:\n");
    for (int i = 0; i < TASKS; i++) {
        printf("Task %d: %s\n", i, tasks[i] ? "processed" : "not processed");
    }

    return 0;
}
