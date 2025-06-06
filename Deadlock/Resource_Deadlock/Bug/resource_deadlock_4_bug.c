#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <unistd.h>

#define N_FILES 6

omp_lock_t cache_lock;
omp_lock_t warning_lock;

int deps[N_FILES];
int optimization_cache[N_FILES] = {0};
int syntax_errors[N_FILES] = {0};
int global_warning_count = 0;

void prepare(int file_id) {
    optimization_cache[file_id] = 0;
}

void parse(int file_id) {
    printf("[File %d] Parsing...\n", file_id);
    usleep(100000 + rand() % 150000);

    omp_set_lock(&cache_lock);
    prepare(file_id);

    if (rand() % 5 == 0) {
        printf("[File %d] Syntax Error found!\n", file_id);
        syntax_errors[file_id] = 1;
        omp_set_lock(&warning_lock);
        global_warning_count++;
        omp_unset_lock(&warning_lock);
        printf("[File %d] Warning: Warning detected while parsing.\n", file_id);
    } else {
        printf("[File %d] Parsing successful.\n", file_id);
    }

    omp_unset_lock(&cache_lock);
}

void codegen(int file_id) {
    if (syntax_errors[file_id]) return;

    printf("[File %d] Generating code...\n", file_id);
    usleep(100000 + rand() % 100000);
    printf("[File %d] Code generation complete.\n", file_id);
}

void optimize(int file_id) {

    printf("[File %d] Optimizing...\n", file_id);
    usleep(120000 + rand() % 120000);

    if (syntax_errors[file_id]) {
        omp_set_lock(&warning_lock);
        global_warning_count++;
        printf("[File %d] Warning: File contains syntax error. Optimization unedfined.\n", file_id);
        omp_set_lock(&cache_lock);
        optimization_cache[file_id] = 2;
        omp_unset_lock(&cache_lock);
        omp_unset_lock(&warning_lock);
    } else {
        omp_set_lock(&cache_lock);
        optimization_cache[file_id] = 1;
        omp_unset_lock(&cache_lock);
    }

    printf("[File %d] Optimization complete.\n", file_id);
}

void global_analysis() {
    printf("[Global] Starting global analysis...\n");
    usleep(200000);
    printf("[Global] Finished global analysis of compiled modules.\n");
}

int main() {
    srand(omp_get_wtime());
    omp_init_lock(&cache_lock);
    omp_init_lock(&warning_lock);

    #pragma omp parallel num_threads(6)
    {
        for (int i = 0; i < N_FILES; ++i) {
            #pragma omp task
            parse(i);

            #pragma omp task
            optimize(i);

            #pragma omp task
            codegen(i);
        }

        global_analysis();

        #pragma omp barrier
    }

    omp_destroy_lock(&cache_lock);
    omp_destroy_lock(&warning_lock);

    printf("\n=== Compilation Summary ===\n");
    for (int i = 0; i < N_FILES; ++i) {
        if (syntax_errors[i])
            printf("File %d: Syntax error\n", i);
        else
            printf("File %d: Compiled, Optimized = %d\n", i, optimization_cache[i]);
    }
    printf("Total Warnings: %d\n", global_warning_count);
    return 0;
}