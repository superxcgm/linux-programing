#include <stdio.h>
#include <string.h>

void foo(char *bar) {
    char c[12];
    strcpy(c, bar); // no bounds checking...
}

int main(void) {
    foo("0123456789123456");
    printf("finish");
    return 0;
}