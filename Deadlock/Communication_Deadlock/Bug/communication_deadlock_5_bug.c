#include <stdio.h>
#include <omp.h>

int main() {
    int message = 0;
    int msgs_sent = 0;
    int msgs_received = 0;

    omp_set_num_threads(2);

    #pragma omp parallel shared(message, msgs_sent, msgs_received)
    {
        int tid = omp_get_thread_num();

        if (tid == 0) {
            for (int i = 1; i <= 5; ++i) {
                while (!msgs_received) {
                    #pragma omp flush(msgs_received)
                }

                message = i + 1;

                msgs_sent = 1;
                msgs_received = 0;

                #pragma omp flush(message, msgs_sent, msgs_received)
                printf("Sent message %d\n", message);
            }
        } else if (tid == 1) {
            for (int i = 1; i <= 5; ++i) {
                while (!msgs_sent) {
                    #pragma omp flush(msgs_sent)
                }

                #pragma omp flush(message)
                printf("Received message %d\n", message);

                msgs_sent = 0;
                msgs_received = 1;

                #pragma omp flush(msgs_sent, msgs_received)
            }
        }
    }

    return 0;
}