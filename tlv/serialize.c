#include "./serialize.h"
#include <memory.h>
#include <stdlib.h>

void init_serialized_buffer(ser_buff_t **b) {
    (*b) = malloc(sizeof(ser_buff_t));
    (*b)->b = malloc(SERIALIZE_BUFFER_DEFAULT_SIZE);
    (*b)->next = 0;
    (*b)->size = SERIALIZE_BUFFER_DEFAULT_SIZE;
    (*b)->checkpoint = 0;
}

void init_serialized_buffer_of_defined_size(ser_buff_t **b, int size) {
    (*b) = malloc(sizeof(ser_buff_t));
    (*b)->b = malloc(size);
    (*b)->next = 0;
    (*b)->size = size;
    (*b)->checkpoint = 0;
}

void serialize_string(ser_buff_t *b, char *data, int nbytes) {
    if (!b) {
        return;
    }
    int available_size = b->size - b->next;
    char is_resize = 0;

    while (available_size < nbytes) {
        b->size *= 2;
        available_size = b->size - b->next;
        is_resize = 1;
    }

    if (is_resize) {
        b->b = realloc(b->b, b->size);
    }

    memcpy(b->b + b->next, data, nbytes);
    b->next += nbytes;
}

int is_serialized_buffer_empty(ser_buff_t *b) {
    if (!b) {
        return 0;
    }
    return b->next == 0 ? 0 : -1;
}

void de_serialize_string(char *dest, ser_buff_t *b, int val_size) {
    if (is_serialized_buffer_empty(b)) {
        return;
    }
    memcpy(dest, b->b + b->next, val_size);
    b->next += val_size;
}

void copy_in_serialized_buffer_by_offset(ser_buff_t *b, int size, char *value, int offset) {
    memcpy(b->b + offset, value, size);
}

void mark_checkpoint_serialize_buffer(ser_buff_t *b) {
    b->checkpoint = b->next;
}

int get_serialize_buffer_checkpoint_offset(ser_buff_t *b) {
    return b->checkpoint;
}

void serialize_buffer_skip(ser_buff_t *b, int skip_size) {
    int target = b->next + skip_size;
    if (target > b->size) {
        b->b = realloc(b->b, target + 5);
    }
    if (target >= 0) {
        b->next = target;
    }
}

void free_serialize_buffer(ser_buff_t *b) {
    if (b) {
        free(b->b);
        free(b);
    }
}

