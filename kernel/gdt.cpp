#include "gdt.h"

inline void lgdt(gdt_descriptor gdtr)
{
    asm("lgdt %0"::"g"(gdtr));
}

// void set_GDT(int index,global_descriptor gd)
// {
    
// }