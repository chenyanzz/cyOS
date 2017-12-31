#include "timer.h"
#include "printf.h"
#include "asm.h"
#include "types.h"

u32 ticks;

bool init_timer()
{
	ticks=0;
	set_timer(TICK_PER_SECOND);
	start_timer();

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

void start_timer()
{
	//打开PIC的时钟中断IRQ0
	outb(0x21,inb(0x21) & 0xfe);
}

extern "C"
void timer_tick()
{
	ticks++;
	
	//accept next tick
	outb(0x20, 0x20);
}