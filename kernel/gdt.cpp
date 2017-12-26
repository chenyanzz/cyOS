#include "gdt.h"

global_descriptor GDT[GDT_NUM];

inline void lgdt(gdt_descriptor gdtr)
{
    asm("lgdt [%0]" ::"a"(&gdtr));
}

// void set_GDT(int index,global_descriptor gd)
// {
    
// }