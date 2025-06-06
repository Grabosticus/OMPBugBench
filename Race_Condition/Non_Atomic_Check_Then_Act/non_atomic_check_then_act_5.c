/*
Simulates a Bank account withdrawal.
Data Race Lines: 21 vs. 25
Difficulty: Easy
*/

#include <stdio.h>
#include <omp.h>

#define INITIAL_BALANCE 100
#define WITHDRAW_AMOUNT 30
#define NUM_THREADS 4

int main() {
    int balance = INITIAL_BALANCE;

#pragma omp parallel num_threads(NUM_THREADS) shared(balance)
    {
        int thread_id = omp_get_thread_num();

        if (balance >= WITHDRAW_AMOUNT) {

            #pragma omp critical
            {
                balance -= WITHDRAW_AMOUNT;
                printf("Thread %d: Withdrawal successful. New balance = %d\n", thread_id, balance);
            }
        } else {
            printf("Balance not high enough.\n");
        }
    }

    printf("\nFinal balance: %d\n", balance);
    return 0;
}
