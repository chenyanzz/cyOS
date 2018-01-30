#include "timer.h"
#include "printf.h"
#include "asm.h"
#include "types.h"
#include "kernel/interrupt/irq.h"

u32 ticks;

bool init_timer()
{
	ticks=0;
	set_timer(TICK_PER_SECOND);
	
	return true;
}

void set_timer(int freq)
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

extern "C"
void timer_tick()
{
	ticks++;

	///@note 因为不再iret，要保持堆栈平衡
	asm(
		"pop cx;"//pop eip
		"pop ecx;"//pop cs
		"pop ecx;"//pop eflags
		:::"ecx"
	);

	///@note 注意！！！必须先接受下一次中断再调度，不然会死在线程里
	accept_new_irq();

	schedule();
}