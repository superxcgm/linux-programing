#ifndef TLV_SERIALIZE_H
#define TLV_SERIALIZE_H

typedef struct serialized_buffer {
#define SERIALIZE_BUFFER_DEFAULT_SIZE 512
    void *b;
    int size;
    int next;
    int checkpoint;
} ser_buff_t;

void init_serialized_buffer(ser_buff_t **b);

void init_serialized_buffer_of_defined_size(ser_buff_t **b, int size);

void serialize_string(ser_buff_t *b, char *data, int nbytes);

int is_serialized_buffer_empty(ser_buff_t *b);

void de_serialize_string(char *dest, ser_buff_t *b, int val_size);

void copy_in_serialized_buffer_by_offset(ser_buff_t *b, int size, char *value, int offset);

void mark_checkpoint_serialize_buffer(ser_buff_t *b);

int get_serialize_buffer_checkpoint_offset(ser_buff_t *b);

void serialize_buffer_skip(ser_buff_t *b, int skip_size);

void free_serialize_buffer(ser_buff_t *b);

#endif //TLV_SERIALIZE_H
