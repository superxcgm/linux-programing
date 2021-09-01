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

#define ITERATE_LIST_BEGIN(list_ptr, node_ptr) \
{                                              \
  dll_node_t *_next = NULL;                    \
  node_ptr = list_ptr->head;                   \
  for (; node_ptr != NULL; node_ptr = _next) { \
    if (!node_ptr) break;                      \
      _next = node_ptr->next;

#define ITERATE_LIST_END }}
