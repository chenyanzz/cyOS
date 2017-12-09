#ifndef ASM_H
#define ASM_H

#include "types.h"
/*
汇编函数宏的实现
*/

#define asm __asm__ __volatile__

inline void cli();
inline void sti();

inline byte inb(u16 port);
inline void outb(u16 port, byte b);
inline void outw(u16 port, word b);

#endif