#include "asm.h"

#include "printf.h"
void cli() { asm("cli"); }

void sti() { asm("sti"); }

void nop() { asm("nop"); }

void hlt() { asm("hlt"); }



byte inb(word port)
{
    byte b;
    asm("push ax;push dx;");
    asm("mov dx,%0" ::"g"(port));
    asm(
        "in al, dx;"
        "mov %0, al;"

        : "=g"(b));
    asm("pop dx;pop ax;");
    // printf("read port %xd:%xd\n",(int)port,(int)b);
    return b;
}

void outb(word port, byte b)
{
    asm("push ax;push dx;");
    asm("mov dx,%0" ::"g"(port));
    asm(
        "mov al, %0;"
        "out dx, al;"

        ::"g"(b));
    asm("pop dx;pop ax;");
    // printf("write port %xd:%xd\n",(int)port,b);
}

word inw(word port)
{
    static word w;
    asm("push ax;push dx;");
    asm("mov dx,%0" ::"g"(port));
    asm(
        "in ax, dx;"
        "mov %0, ax;"

        : "=g"(w));
    asm("pop dx;pop ax;");
    // printf("read port %xd:%xd\n",(int)port,(int)w);
    return w;
}

void outw(word port, word w)
{
    asm("push ax;push dx;");
    asm("mov dx,%0" ::"g"(port));
    asm(
        "mov ax, %0;"
        "out dx, ax;"

        ::"g"(w));
    asm("pop dx;pop ax;");
    // printf("write port %xd:%xd\n",(int)port,(int)w);
}