#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

typedef struct Node {
    int val;
    omp_lock_t data;
    struct Node* next;
} Node;

Node* create_node(int val) {
    omp_lock_t data;
    omp_init_lock(&data);
    Node* new_node = malloc(sizeof(Node));
    if (!new_node) {
        exit(EXIT_FAILURE);
    }
    new_node->val = val;
    new_node->data = data;
    new_node->next = new_node;
    return new_node;
}

Node* insert(Node* node, int val) {
    Node* new_node = create_node(val);
    new_node->next = node->next;
    node->next = new_node;
    return new_node;
}

void operate(Node *list) {
    omp_set_lock(&(list->data));
    list->val += omp_get_thread_num();
    omp_set_lock(&((list->next)->data));
    list->val += (list->next)->val + omp_get_thread_num();
    omp_unset_lock(&((list->next)->data));
    omp_unset_lock(&(list->data));
}

void free_list(Node* start) {
    if (!start) return;

    Node* current = start->next;
    while (current != start) {
        Node* tmp = current;
        current = current->next;
        omp_destroy_lock(&(tmp->data));
        free(tmp);
    }
    omp_destroy_lock(&(start->data));
    free(start);
}

int main() {
    omp_set_num_threads(3);

    Node* list = create_node(1);
    Node* list2 = insert(list, 2);
    Node* list3 = insert(list2, 3);

    #pragma omp parallel
    {
        switch(omp_get_thread_num()) {
            case 0:
                operate(list);
                break;
            case 1:
                operate(list2);
                break;
            case 2:
                operate(list3);
                break;
        }
    }

    printf("1: %d\n", list->val);
    printf("2: %d\n", list2->val);
    printf("3: %d\n", list3->val);

    free_list(list);

    return 0;
}