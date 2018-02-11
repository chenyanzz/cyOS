#include "idt.h"
#include "stdlib.h"
#include "asm.h"
#include "stdio.h"
#include "deal_int.h"
#include "irq.h"

Interrupt_Decriptor IDT[IDT_NUM];

inline void lidt(Idt_Descriptor &idtr)
{
	asm("lidt [%0]" ::"a"(&idtr));
}

void set_idt_item(u8 index, Interrupt_Decriptor item)
{
	memcpy(&IDT[index], &item, sizeof(Interrupt_Decriptor));
}

void set_gate(u8 index, INTERRUPT_CALLBACK callback, GateType gatetype)
{
	u32 func = (u32)callback;
	Interrupt_Decriptor item;
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
	static Idt_Descriptor IDTR;

	for(int i=0;i<IDT_NUM;i++)
	{
		set_gate(i,deal_unexpected_int,FAULT);
	}

	IDTR.addr = IDT;
	IDTR.size = IDT_NUM * 8;
	lidt(IDTR);


	//设置各种中断函数
	set_gate(13,deal_int_13,FAULT);//一般性保护异常
	set_gate(0,deal_int_0,FAULT);//除0报错

	setup_irq();



	return true;
}

