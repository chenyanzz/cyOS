#include "timer.h"
#include "printf.h"
#include "asm.h"
#include "types.h"
#include "irq.h"

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
	/*注意！！！必须先接受下一次中断再调度，不然会死在线程里*/

	//接受下一个中断
	accept_new_irq();

	schedule();
}