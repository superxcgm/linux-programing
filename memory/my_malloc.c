#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

void *my_malloc(int size) {
    struct meta_block_t *p;
    p = (struct meta_block_t *) sbrk(0);

    if (sbrk(size + sizeof(meta_block_t)) == NULL) {
        return NULL;
    }
    p->size = size;
    p->next = NULL;
    p->is_free = FALSE;
    return p + 1;
}

void my_free(int nbytes) {
    assert(nbytes > 0);
    sbrk(nbytes * -1);
}

int main(void) {
    char *p = (char *) my_malloc(20);
    strncpy(p, "hello", strlen("hello"));
    printf("p = %s\n", p);
    my_free(20);

    return 0;
}