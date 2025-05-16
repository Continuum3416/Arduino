#include <stdio.h> 
#include <stdint.h>

int main(int argc, char *argv[])
{
    printf("Hello, my name is Don\n");
    const uint32_t n = 5;
    const uint32_t size = (n << 1) - 1;
    for(uint32_t i = 0; i < size; ++i) {
        for(uint32_t j = 0; j < size; ++j) {
            putchar((j == i || j == size - 1 - i) ? '*' : ' ');
        }
        putchar('\n');
    }
    return 0;
}

// gcc -o bin/display display.c; ./bin/display