#include "dll.h"
#include <memory.h>
#include <stdlib.h>

dll_t *
get_new_dll() {
    dll_t *dll = calloc(1, sizeof(dll_t));
    dll->head = NULL;
    return dll;
}

int
add_data_to_dll(dll_t *dll, void *app_data) {
    if (!dll || !app_data)
        return -1;

    dll_node_t *new_node = calloc(1, sizeof(dll_node_t));
    new_node->prev = NULL;
    new_node->next = NULL;
    new_node->data = app_data;

    if (dll->head == NULL) {
        dll->head = new_node;
        return 0;
    } else {
        dll->head->prev = new_node;
        new_node->next = dll->head;
        dll->head = new_node;
        return 0;
    }
}