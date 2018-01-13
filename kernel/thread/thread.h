#ifndef THREAD_H
#define THREAD_H

#include "../types.h"

#define MAX_THREADS 10000

struct TCB//thread control block线程控制块
{
    id_t id;

    //栈指针
    SEGMENT ss;
    void* esp;

    char name[31];

    bool isRunning;
};

/**
 * 
 */
id_t create_thread(TCB* tcb,RUNNABLE proc);

#endif