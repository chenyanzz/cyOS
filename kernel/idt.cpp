#include "idt.h"
#include "stdlib.h"
#include "asm.h"
#include "stdio.h"
#include "deal_int.h"


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
	u32 func = (u32)callback;
	interrupt_decriptor item;
	item.segment_selector = KNL_PROGRAM_SEG;
	item.offset_0_15 = (u16)func;
	item.offset_16_31 = (u16)(func >> 15);
	item.present = true;
	item.rpl = KERNEL;
	item.zero = 0;
	item.type = gatetype;
	set_idt_item(index, item);
}

bool init_IDT()
{

	//初始化idt表
	memset(IDT, 0, sizeof(IDT));
	memset(&IDTR, 0, sizeof(IDTR));

	IDTR.addr = IDT;
	IDTR.size = IDT_NUM * 8;
	lidt(IDTR);

	//设置各种中断函数
	set_gate(13,deal_int_13,FAULT);//一般性保护异常
	set_gate(0,deal_int_0,FAULT);//除0报错
	set_gate(IRQ(0),deal_irq_0,INT);//时钟中断

	sti();
	return true;
}