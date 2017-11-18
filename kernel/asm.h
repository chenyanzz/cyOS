#ifndef ASM_H
#define ASM_H
/*
汇编函数宏的实现
*/

#define asm __asm__ __volatile__

inline void cli();
inline void sti();

#endif