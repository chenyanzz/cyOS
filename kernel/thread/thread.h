#ifndef THREAD_H
#define THREAD_H

#include "../types.h"

typedef int16    SEGMENT;
typedef int32    THREAD_ID;
typedef void (*RUNNABLE)();

struct TCB//thread control block线程控制块
{
    THREAD_ID id;

    //栈指针
    SEGMENT ss;
    MEM_ADDR esp;
};

void test_thread();
void switch_to_t1();
void switch_to_t2();

#endif