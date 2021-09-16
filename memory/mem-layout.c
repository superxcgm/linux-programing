#include <stdio.h>

int global1_bss;
int global2_bss;

int global_data = 5;

int main(void) {
    static int st = 6;
    return 0;
}
