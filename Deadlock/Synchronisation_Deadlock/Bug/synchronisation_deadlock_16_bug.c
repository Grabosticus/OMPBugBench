#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define N 1000
#define CHUNKSIZE 50
#define THREADS 4

int update = 0;
omp_lock_t update_lock;
int interrupted = -1;

void read_update() {
    omp_set_lock(&update_lock);
    printf("Thread %d informed about update %d\n", omp_get_thread_num(), update);
    omp_unset_lock(&update_lock);
    #pragma omp barrier
}

int call_api() {
    int api_result = rand(); // mock API result
    #pragma omp barrier
    return api_result;
}

void get_update() {
    omp_set_lock(&update_lock);
    update = call_api();
    omp_unset_lock(&update_lock);
}

void save_result(int sums[], int n) {
    int sum = 0;
    for (int i = 0; i < n; i++) {
        sum += sums[i];
    }
    #pragma omp barrier
    printf("Sum: %d\n", sum);
}

void interrupt() {
    interrupted = rand() % THREADS;
    if (interrupted == 0) {
        interrupted = 1;
    }
}

int main() {
    srand(time(NULL));
    omp_init_lock(&update_lock);
    int arr[N];
    int segment_sums[N / CHUNKSIZE] = {0};
    int idx = 0;

    for (int i = 0; i < N; i++) {
        arr[i] = i + 1;
    }

    omp_set_num_threads(THREADS);

    #pragma omp parallel
    {
        int local_sum = 0;
        int thread_id = omp_get_thread_num();

        #pragma omp for schedule(static, CHUNKSIZE)
        for (int i = 0; i < N; i++) {
            local_sum += arr[i];
            if (thread_id == interrupted)
                continue;

            if ((i + 1) % CHUNKSIZE == 0) {
                #pragma omp critical
                {
                    segment_sums[idx] = local_sum;
                    printf("Thread %d writing segment_sums[%d] = %d\n", thread_id, idx, local_sum);
                    get_update();
                    idx++;
                    if (rand() % N > 500) {
                        interrupt();
                    }
                }
                local_sum = 0;
            }
            read_update();
        }

        #pragma omp master
        {
            printf("\nMaster thread (%d) printing final results:\n", thread_id);
            for (int i = 0; i < idx; i++) {
                printf("Segment %d sum = %d\n", i, segment_sums[i]);
            }
            save_result(segment_sums, idx);
        }
    }

    omp_destroy_lock(&update_lock);

    return 0;
}