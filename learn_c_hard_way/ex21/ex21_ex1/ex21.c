#include <stdint.h>
#include <stdio.h>

int main(void) {
    int_least64_t int_least64 = 1;
    uint_least64_t uint_least64 = 2;
    int_least8_t int_least8 = 3;
    uint_least8_t uint_least8 = 4;

    printf("%ld, %lu", int_least64, uint_least64);

    printf("%d, %u", int_least8, uint_least8);
    return 0;
}