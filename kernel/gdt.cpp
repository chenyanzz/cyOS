#include "gdt.h"
#include "stdlib.h"

global_descriptor GDT[GDT_NUM];

inline void lgdt(gdt_descriptor& gdtr) {
    asm("lgdt [%0]"::"a"(&gdtr));
}

void set_gdt_item(u8 index, global_descriptor item) {
    memcpy(&GDT[index], &item, sizeof(global_descriptor));
}

bool init_GDT() {
    memset(GDT,0,sizeof(GDT));
    set_gdt_item(0, {0});
    set_gdt_item(1, make_descriptor(memory_total_size, 0, CODE_RW, KERNEL));
    set_gdt_item(2, make_descriptor(memory_total_size, 0, DATA_RW, KERNEL));
    static gdt_descriptor gdtr;
    gdtr.addr = GDT;
    gdtr.size = sizeof(GDT);
    lgdt(gdtr);
    return true;
}