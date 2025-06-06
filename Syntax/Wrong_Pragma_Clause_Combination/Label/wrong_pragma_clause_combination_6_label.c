/*
A complex parallel program processing resources with mutliple threads. 
Bug Lines: 69, 65, 55 (order of keywords changed and "depend" and "shared" got the argument of each other),
39 (wrong order of keywords; collapse() argument is written after "for"),
35 (wrong order of operations, default() and shared() got each others arguments)
Difficulty: Hard
*/

#include <stdlib.h>
#include <omp.h>
#include <stdio.h>

#define ITERATIONS 1000

typedef struct {
    int id;
    omp_lock_t lock;
    int data;
} Resource;

void process_resource(Resource *res, int thread_id) {
    omp_set_lock(&res->lock);
    res->data += thread_id;
    printf("Thread %d processed Resource %d with data %d\n", thread_id, res->id, res->data);
    omp_unset_lock(&res->lock);
}

int compute(int i, int j) {
    return i * j + (i % 3) - (j % 5);
}

int main() {
    int sum = 0;
    Resource resources[4];

    for (int i = 0; i < 4; ++i) {
        resources[i].id = i;
        resources[i].data = 0;
        omp_init_lock(&resources[i].lock);
    }

    int iter = ITERATIONS;
    #pragma omp parallel default(none) shared(resources, sum) firstprivate(iter) num_threads(4) \
                          proc_bind(spread)
    {
        int thread_id = omp_get_thread_num();

        #pragma omp for collapse(2) schedule(static, 10) nowait reduction(+:sum)
        for (int i = 0; i < ITERATIONS; ++i) {
            for (int j = 0; j < ITERATIONS; ++j) {
                int value = compute(i, j);
                sum += value;

                if (value % 100 == 0) {
                    process_resource(&resources[value % 4], thread_id);
                }
            }
        }

        #pragma omp single
        {
            int k = 0;
            while (k < 4) {
                #pragma omp task depend(out: resources[k]) shared(resources) firstprivate(k, thread_id)
                {
                    omp_set_lock(&resources[k].lock);
                    resources[k].data += k * thread_id;
                    printf("Task: Resource %d updated to %d by thread %d\n", resources[k].id, resources[k].data, thread_id);
                    omp_unset_lock(&resources[k].lock);
                }
                k++;
            }

            #pragma omp taskwait
            printf("All tasks completed by thread %d\n", thread_id);
        }

        #pragma omp critical(final_report)
        {
            printf("Thread %d reporting: partial sum = %d\n", thread_id, sum);
        }
    }

    for (int i = 0; i < 4; ++i) {
        omp_destroy_lock(&resources[i].lock);
    }

    printf("Final sum = %d\n", sum);
    for (int i = 0; i < 4; ++i) {
        printf("Final Resource %d data = %d\n", resources[i].id, resources[i].data);
    }

    return 0;
}
