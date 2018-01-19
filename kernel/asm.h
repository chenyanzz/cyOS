#ifndef ASM_H
#define ASM_H

#include "types.h"
#include "memory/page.h"

/*汇编函数宏的实现*/

#define asm __asm__ __volatile__

void cli();
void sti();

void nop();
void hlt();

byte inb(word port);
void outb(word port, byte b);

word inw(word port);
void outw(word port, word b);

#define int(index) asm("int "#index)

#define push_reg()	\
	asm(			\
		"pushad;"	\
		"push ds;"	\
		"push es;"	\
		"push fs;"	\
		"push gs;"	\
		"pushf;"	\
    )

#define pop_reg()	\
	asm(			\
		"popf;"		\
		"pop gs;"	\
		"pop fs;"	\
		"pop es;"	\
		"pop ds;"	\
		"popad;"	\
	)

/*汇编程序里的变量指针*/

#define p_cursor_x ((u8 *)0x90000)
#define p_cursor_y ((u8 *)0x90001)
#define p_extend_memory_size ((u16 *)0x90003)
#define p_firstChar ((u8 *)0xB8000)

#define p_count_mem_list ((u16 *)0x90030)
#define p_mem_list_items ((MEM_LIST_ITEM *)0x90040)

/*一些底层宏定义*/
//将段号（0~2047）变成段选择符号
#define gknlseg(n) (SEGMENT)(((n) << 3) + 0b000)
#define gusrseg(n) (SEGMENT)(((n) << 3) + 0x011)

#define lknlseg(n) (SEGMENT)(((n) << 3) + 0b100)
#define lusrseg(n) (SEGMENT)(((n) << 3) + 0b111)

#define KNL_PROGRAM_SEG gknlseg(1)
#define KNL_DATA_SEG gknlseg(2)

/*位操作*/
#define bit(n, nb) (((n) & (1 << nb)) != 0)
#define setb(n, nb) ((n) |= (1 << nb))
#define clrb(n, nb) ((n)&= ~(1<<nb))

#define close_int() u8 irq_stat = stop_all_irq();	cli()
#define start_int() sti();	restore_irq(irq_stat)

#endif