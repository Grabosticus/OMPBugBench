#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define MAX_DEPTH 5
#define MAX_CHILDREN 3

typedef struct Node {
    int value;
    int depth;
    struct Node* children[MAX_CHILDREN];
} Node;

omp_lock_t global_sum_lock;
int global_sum = 0;

Node* create_tree(int depth) {
    if (depth == 0) return NULL;

    Node* node = (Node*)malloc(sizeof(Node));
    node->value = rand() % 100;
    node->depth = depth;

    for (int i = 0; i < MAX_CHILDREN; ++i) {
        node->children[i] = create_tree(depth - 1);
    }

    return node;
}

void free_tree(Node* node) {
    if (!node) return;
    for (int i = 0; i < MAX_CHILDREN; ++i)
        free_tree(node->children[i]);
    free(node);
}

void compute_weighted_sum(Node* node) {
    if (!node) return;

    int tid = omp_get_thread_num();

    omp_set_lock(&global_sum_lock);
    if (node->value > 90) goto rec;
    global_sum += node->value * node->depth;
    omp_unset_lock(&global_sum_lock);

    rec:
        for (int i = 0; i < MAX_CHILDREN; ++i) {
            if (node->children[i]) {
                #pragma omp task firstprivate(i)
                compute_weighted_sum(node->children[i]);
            }
        }

    #pragma omp taskwait
}

int main() {
    srand(5);
    omp_init_lock(&global_sum_lock);

    Node* root = create_tree(MAX_DEPTH + 1);

    double start = omp_get_wtime();

    #pragma omp parallel num_threads(4)
    {
        #pragma omp single
        compute_weighted_sum(root);
    }

    double end = omp_get_wtime();

    printf("Final weighted sum: %d\n", global_sum);
    printf("Time taken: %f seconds\n", end - start);

    free_tree(root);

    omp_destroy_lock(&global_sum_lock);

    return 0;
}
