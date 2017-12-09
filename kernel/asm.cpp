#include "asm.h"

void cli()
{
    asm("cli");
}

void sti()
{
    asm("sti");
}

byte inb(u16 port)
{
    byte b;

    asm("mov dx,%0"::"g"(port));
    asm(
        "in al, dx;"
        "mov %0, al;"
        
        :"=g"(b)::"dx","ax"
    );
  
    return b;
}

void outb(u16 port, byte b)
{
    asm("mov dx,%0"::"g"(port));
    asm(
        "mov al, %0;"
        "out dx, al;"
        
        ::"g"(b):"dx","ax"
    );
}

void outw(u16 port, word w)
{
    asm("mov dx,%0"::"g"(port));
    asm(
        "mov ax, %0;"
        "out dx, ax;"
        
        ::"g"(w):"dx","ax"
    );
}