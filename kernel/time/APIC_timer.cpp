#include "APIC_timer.h"
#include "printf.h"
#include "asm.h"
#include "types.h"
#include "kernel/interrupt/irq.h"
#include "time.h"

bool init_APIC_timer()
{
	ticks=0;
	set_APIC_timer(APIC_TIMER_TICKS_PER_SECOND);
	return true;
}

void set_APIC_timer(int freq)
{
	//计算divisor
	u32 divisor = 1193180 / freq;
	//写入8253的控制寄存器
	outb(0x43, 0x36);
	//写入频率低8位到计数器0
	outb(0x40, divisor & 0xff);
	//写入频率高8位到计数器0
	outb(0x40, divisor >> 8);
}

#include "thread/thread.h"
#include "time.h"

extern "C"
void APIC_timer_tick()
{
	ticks++;
	addTime();
	accept_new_irq(APIC_TIMER_IRQ);
}