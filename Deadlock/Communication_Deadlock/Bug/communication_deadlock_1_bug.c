#include <stdio.h>
#include <omp.h>

#define NUM_THREADS 4

int main() {
    int messages[NUM_THREADS] = {0};
    int message_sent_flags[NUM_THREADS] = {0};

    omp_set_num_threads(NUM_THREADS);

    #pragma omp parallel shared(messages, message_sent_flags)
    {
        int thread_id = omp_get_thread_num();
        int next = (thread_id + 1) % NUM_THREADS;
        int prev = (thread_id - 1 + NUM_THREADS) % NUM_THREADS;
        
        while (message_sent_flags[thread_id] == 0) {
            #pragma omp flush(message_sent_flags)
        }

        #pragma omp flush(messages)

        printf("Thread %d: received message %d from thread %d\n", thread_id, messages[thread_id], prev);

        int msg = thread_id * 100;
        printf("Thread %d: sending message %d to thread %d\n", thread_id, msg, next);

        messages[next] = msg;

        #pragma omp flush(messages, message_sent_flags)

        message_sent_flags[next] = 1;

        #pragma omp flush(message_sent_flags)
    }

    return 0;
}