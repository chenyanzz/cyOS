#include "asm.h"
#include "printf.h"
void cli() { asm("cli"); }

void sti() { asm("sti"); }

void nop() { asm("nop"); }

byte inb(word port)
{
    byte b;

    asm("mov dx,%0"::"g"(port));
    asm(
        "in al, dx;"
        "mov %0, al;"
        
        :"=g"(b)::"dx","ax"
    );
    printf("read port %d:%d\n",(int)port,(int)b);
    return b;
}

void outb(word port, byte b)
{
    asm("mov dx,%0"::"g"(port));
    asm(
        "mov al, %0;"
        "out dx, al;"
        
        ::"g"(b):"dx","ax"
    );
    printf("write port %d:%d\n",(int)port,(int)b);
}

void outw(word port, word w)
{
    asm("mov dx,%0"::"g"(port));
    asm(
        "mov ax, %0;"
        "out dx, ax;"
        
        ::"g"(w):"dx","ax"
    );
    printf("write port %d:%d\n",(int)port,(int)w);
}