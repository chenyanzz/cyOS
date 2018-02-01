#include "stdlib.h"
#include "assert.h"

void memset(void *addr, char val, int size) {
    if (size <= 0)return;
    asm("pushf");
    asm("cld");
    asm("rep stosb"::"c"(size), "a"(val), "D"(addr));
    asm("popf");
}

void memcpy(void *to, void *from, int size) {
    if (size <= 0)return;
    asm("pushf");
    asm("cld");
    asm("rep movsb"::"c"(size), "S"(from), "D"(to));
    asm("popf");
}
