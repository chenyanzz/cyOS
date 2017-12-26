#include "idt.h"
#include "stdlib.h"
#include "asm.h"
#include "stdio.h"

interrupt_decriptor IDT[IDT_NUM];
idt_descriptor IDTR;

inline void lidt(idt_descriptor idtr)
{
	asm("lidt [%0]" ::"a"(&idtr));
}

void set_idt_item(u8 index, interrupt_decriptor item)
{
	memcpy(&IDT[index], &item, sizeof(interrupt_decriptor));
}

void set_gate(u8 index, INTERRUPT_CALLBACK callback, GateType gatetype)
{
	u32 func = (u32)(void*)callback;
	interrupt_decriptor item;
	item.segment_selector=KNL_PROGRAM_SEG;
	item.offset_0_15=(u16)func;
	item.offset_16_31=(u16)(func>>15);
	item.present=true;
	item.rpl=KERNEL;
	item.S=0;
	item.zero=0;
	item.type=gatetype;
	set_idt_item(index,item);
}

void TEST()
{
	printf("int called!\n");
	iret();
}

bool init_IDT()
{
	memset(IDT,0,sizeof(IDT));
	set_gate(0x50,TEST,INT_GATE);
	IDTR.addr = IDT;
	IDTR.size = 512*8;
	lidt(IDTR);
	return true;
}