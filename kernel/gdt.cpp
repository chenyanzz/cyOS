#include "gdt.h"
#include "stdlib.h"

global_descriptor GDT[GDT_NUM];

inline void lgdt(gdt_descriptor& gdtr) {
    asm("lgdt [%0]"::"a"(&gdtr));
}

void set_gdt_item(u8 index, global_descriptor item) {
    memcpy(&GDT[index], &item, sizeof(global_descriptor));
}

segment_descriptor make_descriptor(int limit, int baseaddr, SEGMENT_TYPE segment_type, RPL rpl)
{
    segment_descriptor sd;
    sd.present=true;

    sd.baseaddr_0_23 = baseaddr;
    sd.baseaddr_24_31 = baseaddr>>24;

    int limit_4k = limit/4096;
    sd.seglmt_0_15 = limit_4k;
    sd.seglmt_16_19 = limit_4k>>16;

    sd.segtype = segment_type;

    sd.AVL=0;
    sd.zero=0;
    sd.g = segment_descriptor::UNIT_4K;
    sd.d=segment_descriptor::BIT_32;
    sd.s=segment_descriptor::CODE_DATA;
    sd.rpl = rpl;

    return sd;
}

bool init_GDT() {
    memset(GDT,0,sizeof(GDT));
    set_gdt_item(1, make_descriptor(memory_total_size, 0, CODE_RW, KERNEL));
    set_gdt_item(2, make_descriptor(memory_total_size, 0, DATA_RW, KERNEL));
    static gdt_descriptor gdtr;
    gdtr.addr = GDT;
    gdtr.size = sizeof(GDT);
    lgdt(gdtr);
    return true;
}