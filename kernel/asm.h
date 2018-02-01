#pragma once


#include "types.h"
#include "memory/page.h"

/*汇编函数宏的实现*/

#define asm __asm__ __volatile__

///以下函数对应汇编指令

void cli();
void sti();

void nop();
void hlt();

byte inb(word port);
void outb(word port, byte b);

word inw(word port);
void outw(word port, word b);

#define int(index) asm("int "#index)

/* 一些函数的汇编宏实现 */

///压栈状态
#define push_reg()	\
	asm(			\
		"pushad;"	\
		"push ds;"	\
		"push es;"	\
		"push fs;"	\
		"push gs;"	\
		"pushf;"	\
    )


///出栈状态
#define pop_reg()	\
	asm(			\
		"popf;"		\
		"pop gs;"	\
		"pop fs;"	\
		"pop es;"	\
		"pop ds;"	\
		"popad;"	\
	)

///关闭中断和IRQ
#define close_int() u16 irq_stat = stop_all_irq();	cli()

///开启中断和IRQ（必须先close_int）
#define start_int() restore_irq(irq_stat);	sti()

/*汇编程序里的变量指针*/

#define p_cursor_x ((u8 *)0x90000)
#define p_cursor_y ((u8 *)0x90001)
#define p_extend_memory_size ((u16 *)0x90003)
#define p_firstChar ((u8 *)0xB8000)

#define p_count_mem_list ((u16 *)0x90030)
#define p_mem_list_items ((MEM_LIST_ITEM *)0x90040)

/*一些底层宏定义*/

//将段号（0~2047）变成段选择符号
#define gknlseg(n) (u32)(((n) << 3) + 0b000)
#define gusrseg(n) (u32)(((n) << 3) + 0x011)

#define lknlseg(n) (u32)(((n) << 3) + 0b100)
#define lusrseg(n) (u32)(((n) << 3) + 0b111)

#define KNL_PROGRAM_SEG gknlseg(1)
#define KNL_DATA_SEG gknlseg(2)

/*位操作*/

///获取一位的值
#define bit(n, nb) (((n) & (1 << (nb))) != 0)

///设置一位=1
#define setb(n, nb) ((n) |= (1 << (nb)))

///设置一位=0
#define clrb(n, nb) ((n)&= ~(1<<(nb)))


