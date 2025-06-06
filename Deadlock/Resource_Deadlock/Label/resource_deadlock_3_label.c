/*
Simulates bank account transfers using OpenMP.
Bug: On line 16 and 17 in the bug file "from" is always locked before "to". Therefore, if two threads call transfer(from, to, X)
and transfer(to, from, X), one thread could lock "from" and the other "to" and then both threads wait for each others locks --> Deadlock
Difficulty: Medium
*/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <unistd.h>

#define NUM_ACCOUNTS 3
#define NUM_THREADS 4
#define NUM_TRANSFERS 10

typedef struct {
    int balance;
    omp_lock_t lock;
} BankAccount;

void transfer(BankAccount* from, BankAccount* to, int amount) {
    if (from < to) {
        omp_set_lock(&from->lock);
        omp_set_lock(&to->lock);
    } else {
        omp_set_lock(&to->lock);
        omp_set_lock(&from->lock);
    }

    if (from->balance >= amount) {
        from->balance -= amount;
        to->balance += amount;
    } else {
        printf("Not enough money to transfer\n");
    }

    omp_unset_lock(&from->lock);
    omp_unset_lock(&to->lock);
}

int main() {
    BankAccount accounts[NUM_ACCOUNTS];

    for (int i = 0; i < NUM_ACCOUNTS; i++) {
        accounts[i].balance = 1000;
        omp_init_lock(&accounts[i].lock);
    }

    omp_set_num_threads(NUM_THREADS);

    #pragma omp parallel
    {
        unsigned int seed = omp_get_thread_num();

        for (int i = 0; i < NUM_TRANSFERS; i++) {
            int from_idx = rand_r(&seed) % NUM_ACCOUNTS;
            int to_idx;
            do {
                to_idx = rand_r(&seed) % NUM_ACCOUNTS;
            } while (to_idx == from_idx);

            int amount = (rand_r(&seed) % 200) + 1;

            transfer(&accounts[from_idx], &accounts[to_idx], amount);
        }
    }

    printf("\nFinal balances:\n");
    for (int i = 0; i < NUM_ACCOUNTS; i++) {
        printf("Account %d: %d\n", i, accounts[i].balance);
    }

    for (int i = 0; i < NUM_ACCOUNTS; i++) {
        omp_destroy_lock(&accounts[i].lock);
    }

    return 0;
}
