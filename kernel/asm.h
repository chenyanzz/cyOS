#ifndef ASM_H
#define ASM_H

#include "types.h"
/*
汇编函数宏的实现
*/

#define asm __asm__ __volatile__

void cli();
void sti();

void nop();

byte inb(word port);
void outb(word port, byte b);

word inw(word port);
void outw(word port, word b);

#endif