#include <stdio.h>

int main() {

    for (int i = 0; i < 100; i++) {
        
        __asm__("nop\n\t");
    }
    printf("Hello\n");

    return 0;
}