#include "./serialize.h"
#include <stdio.h>

int main(void) {

    ser_buff_t *ser_buff = NULL;
    init_serialized_buffer(&ser_buff);
    init_serialized_buffer_of_defined_size(&ser_buff, 1024);

    return 0;
}
