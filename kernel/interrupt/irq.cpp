#include "irq.h"
#include "irq.h"
#include "asm.h"
#include "deal_int.h"
#include "idt.h"

#define IRQ(n) (0x20 + (n))

void setup_irq()
{
	stop_all_irq();
	set_gate(IRQ(TIMER_IRQ),deal_irq_0,INT);//时钟
	set_gate(IRQ(KEYBOARD_IRQ),deal_irq_1,INT);//键盘
}

u8 start_irq(u8 index)
{
	u8 oldst = inb(0x21);
	u8 st = oldst;
	clrb(st,index);
	outb(0x21,st);
	return oldst;
}

u8 stop_irq(u8 index)
{
	u8 oldst = inb(0x21);
	u8 st = oldst;
	setb(st,index);
	outb(0x21,st);
	return oldst;
}

u8 stop_all_irq()
{
	u8 oldst = inb(0x21);
	outb(0x21,0b11111111);
	return oldst;
}

u8 restore_irq(u8 mode)
{
	u8 oldst = inb(0x21);
	outb(0x21,mode);
	return oldst;
}

void accept_new_irq()
{
	outb(0x20, 0x20);
}

u8 start_all_irq()
{
	u8 oldst = inb(0x21);

	start_irq(TIMER_IRQ);
	start_irq(KEYBOARD_IRQ);

	return oldst;
}