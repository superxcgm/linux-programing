typedef struct dll_node_ {
    void *data;
    struct dll_node_ *prev;
    struct dll_node_ *next;
} dll_node_t;

typedef struct dll_ {
    dll_node_t *head;
} dll_t;

dll_t *
get_new_dll();

int
add_data_to_dll(dll_t *dll, void *app_data);

int
remove_data_from_dll_by_data_ptr(dll_t *dll, void *data);

int
is_dll_empty(dll_t *dll);

void
drain_dll(dll_t *dll);