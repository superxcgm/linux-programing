#include <stdio.h>

int main(void) {
    unsigned int x = 1;

    char *p = (char *) &x;
    if (*p == 1) {
        printf("Your machine is little-endian(Host order)\n");
    } else {
        printf("Your machine is Big-endian(Network order)\n");
    }

    return 0;
}
