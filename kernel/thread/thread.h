#ifndef THREAD_H
#define THREAD_H

#include "../types.h"

typedef int16    SEGMENT;
typedef int32    THREAD_ID;
typedef void (*RUNNABLE)();

//将段号（0~2047）变成段选择符号
//#define gknlseg(n) (SEGMENT)((n)<<3+0b000)

#define gknlseg(n) ((SEGMENT)((n)<<3+0b000))

#define gusrseg(n) ((SEGMENT)((n)<<3+0x011))

#define lknlseg(n) ((SEGMENT)((n)<<3+0b100))
#define lusrseg(n) ((SEGMENT)((n)<<3+0b111))

#define KPROGRAM_SEG gknlseg(1)
#define KDATA_SEG gknlseg(2)

struct TCB//thread control block线程控制块
{
    THREAD_ID id;

    //栈指针
    SEGMENT ss;
    MEM_ADDR esp;

    char name[31];
};

void test_thread();
void switch_to_t1();
void switch_to_t2();

#endif