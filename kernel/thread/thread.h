#ifndef THREAD_H
#define THREAD_H

#include "../types.h"

#define MAX_THREADS 3

struct TCB //thread control block : 线程控制块
{
    id_t id;

    //栈指针
    SEGMENT ss;
    void *esp;

    char name[31];

    bool isWaiting : 1;
    bool isEnd : 1;
    bool isPresent : 1;
};

/**
 * 初始化线程
 */
bool init_thread();

/**
 * 切换到指定线程
 * @param ptcb 新线程的TCB指针
 */
void switch_to(TCB *ptcb);

/**
 * 创建线程
 * @param proc 新线程的主函数
 * @param name 线程名称，最大31字符
 * @param bWait 是否等待唤醒
 *          默认为false，立即进入可调度状态
 * @return 新线程的id
 */
id_t create_thread(RUNNABLE proc,char* name, bool bWait=false);

/**
 * 切换一次线程
 * 改变current指针
 * 如果没有可用线程则直接返回
 * 
 * @tip： 现在使用的是最简单的FIFO调度
 */
void schedule();

/**
 * 寻找isPresent=false的TCB并返回指针
 * @retval nullptr 如果没有空TCB
 */ 
TCB* findNewTCB();

#endif