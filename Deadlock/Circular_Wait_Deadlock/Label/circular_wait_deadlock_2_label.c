/*
Simulates multiple robots accessing mutliple items in multiple zones using OpenMP.
Bug: If action == 2 for 3 threads, a circular wait could arise. (Thread 0 holds zone_id and wants (zone_id + 1), 
Thread 1 holds (zone_id + 1) and wants (zone_id + 2), Thread 2 holds (zone_id + 2) and wants zone_id)
Difficulty: Medium
*/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <unistd.h>

#define NUM_ZONES 3
#define NUM_ROBOTS 6
#define NUM_TASKS 8

typedef struct {
    int stock;
    omp_lock_t lock;
} Zone;

void robot_task(int id, Zone zones[]) {
    unsigned int seed = id + 123;
    for (int i = 0; i < NUM_TASKS; ++i) {
        int zone_id = rand_r(&seed) % NUM_ZONES;
        int action = rand_r(&seed) % 3;
        int amount = (rand_r(&seed) % 5) + 1;
        int transfer_zone_id = (zone_id + 1) % NUM_ZONES;

        if (action == 2 && zone_id > transfer_zone_id) { // global lock ordering to prevent deadlock
            int tmp = zone_id;
            zone_id = transfer_zone_id;
            transfer_zone_id = tmp;
        }

        omp_set_lock(&zones[zone_id].lock);

        if (action == 0) {
            if (zones[zone_id].stock >= amount) {
                zones[zone_id].stock -= amount;
            } else {
                printf("Item stock too small\n");
            }
        } else if (action == 1) {
            zones[zone_id].stock += amount;
        } else {
            omp_set_lock(&zones[transfer_zone_id].lock);
            if (zones[transfer_zone_id].stock >= 1) {
                zones[zone_id].stock += 1;
                zones[transfer_zone_id].stock -= 1;
            }
            omp_unset_lock(&zones[transfer_zone_id].lock);
        }

        omp_unset_lock(&zones[zone_id].lock);
    }
}

int main() {
    Zone zones[NUM_ZONES];

    for (int i = 0; i < NUM_ZONES; ++i) {
        zones[i].stock = 10;
        omp_init_lock(&zones[i].lock);
    }

    omp_set_num_threads(NUM_ROBOTS + 1);

    #pragma omp parallel shared(zones)
    {
        int tid = omp_get_thread_num();

        if (tid < NUM_ROBOTS) {
            robot_task(tid, zones);

            #pragma omp barrier

            printf("Robot %d waiting at checkpoint.\n", tid);

            #pragma omp barrier
        } else {
            #pragma omp barrier
            printf("\nSupervisor: All robots completed tasks. Evaluating stock levels...\n");
            for (int i = 0; i < NUM_ZONES; ++i) {
                omp_set_lock(&zones[i].lock);
                printf("Zone %d Final Stock: %d\n", i, zones[i].stock);
                omp_unset_lock(&zones[i].lock);
            }

            #pragma omp barrier
        }
    }

    for (int i = 0; i < NUM_ZONES; ++i)
        omp_destroy_lock(&zones[i].lock);

    return 0;
}
