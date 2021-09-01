#include "dll.h"
#include <stdlib.h>
#include <memory.h>

int
remove_data_from_dll_by_data_ptr(dll_t *dll, void *data) {
  dll_node_t *p = dll->head;
  while (p) {
    if (p->data == data) {
      dll_node_t *pre = p->prev;
      dll_node_t *next = p->next;
      if (pre == NULL) {
        dll->head = next;
        next->prev = NULL;
        free(p);
        p = next;
        continue;
      }
      free(p);
      pre->next = next;
      if (next == NULL) {
        return 0;
      }
      next->prev = pre;
      p = pre;
    }
    p = p->next;
  }
  return 0;
}

int
is_dll_empty(dll_t *dll) {
  return dll->head == NULL ? 0 : -1;
}

void
drain_dll(dll_t *dll) {
  dll_node_t *p = dll->head;
  while (p) {
    dll_node_t *next = p->next;
    free(p);
    p = next;
  }
  dll->head = NULL;
}
