/*
Producer-Consumer problem with OpenMP locks.
Bug Lines: 92 (Lock set at wrong time), 83 (lock freed at wrong point), 46, 48 (omp_init_lock_t() used instead of omp_init_lock()) 
Difficulty: Medium
*/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define QUEUE_SIZE 10
#define NUM_PRODUCERS 2
#define NUM_CONSUMERS 2
#define ITEMS_TO_PRODUCE 20

typedef struct {
    int buffer[QUEUE_SIZE];
    int head;
    int tail;
    int count;
} Queue;

int dequeue(Queue* q) {
    int item = q->buffer[q->head];

    q->head = (q->head + 1) % QUEUE_SIZE;

    q->count--;

    return item;
}


void queue_in(Queue* q, int item) {
    q->buffer[q->tail] = item;

    q->tail = (q->tail + 1) % QUEUE_SIZE;

    q->count++;
}

int main() {

    omp_lock_t queue_lock, counter_lock;

    int produced_count = 0;

    int consumed_count = 0;

    Queue queue = {.head = 0, .tail = 0, .count = 0};

    omp_init_lock(&queue_lock);

    omp_init_lock(&counter_lock);


    #pragma omp parallel num_threads(NUM_PRODUCERS + NUM_CONSUMERS)
    {
        int thread_id = omp_get_thread_num();

        if (thread_id < NUM_PRODUCERS) {
            for (int i = 0; i < ITEMS_TO_PRODUCE; ++i) {
                int item = thread_id * 100 + i;

                int added = 0;
                while (!added) {
                    omp_set_lock(&queue_lock);
                    if (queue.count < QUEUE_SIZE) {
                        queue_in(&queue, item);
                        printf("Producer %d produced item %d\n", thread_id, item);
                        added = 1;
                    }
                    omp_unset_lock(&queue_lock);
                }

                omp_set_lock(&counter_lock);
                produced_count++;
                omp_unset_lock(&counter_lock);
            }
        } else {
            int consumed = 0;
            int num = (NUM_PRODUCERS * ITEMS_TO_PRODUCE) / NUM_CONSUMERS;

            for (int consumed = 0; consumed < num; consumed++) {
                int got_item = 0;
                int item = -1;

                omp_set_lock(&queue_lock);
                if (queue.count > 0) {
                    item = dequeue(&queue);
                    got_item = 1;
                }
                omp_unset_lock(&queue_lock);

                if (got_item) {
                    printf("Consumer %d consumed item %d\n", thread_id, item);
                    omp_set_lock(&counter_lock);
                    consumed_count++;
                    omp_unset_lock(&counter_lock);
                }
            }
        }
    }

    printf("\nTotal produced: %d\n", produced_count);
    printf("Total consumed: %d\n", consumed_count);

    omp_destroy_lock(&queue_lock);
    omp_destroy_lock(&counter_lock);

    return 0;
}
