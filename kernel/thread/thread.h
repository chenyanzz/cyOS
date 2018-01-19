#ifndef THREAD_H
#define THREAD_H

#include "../types.h"

#define MAX_THREADS 1000

struct TCB //thread control block : 线程控制块
{
    id_t id;//从1开始，0表示无效

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
 * 退出当前线程的函数
 * @param retCode 返回值
 */
void exit(int retCode=0);

/**
 * 切换一次线程
 * 改变current指针
 * 如果没有可用线程则直接返回
 * @tip： 现在使用的是最简单的FIFO调度
 */
void schedule();

/**
 * 寻找isPresent=false的TCB并返回指针
 * @retval nullptr 如果没有空TCB
 */ 
TCB* findNewTCB();

/**
 * 将current线程删除
 * 再线程退出时被自动调用
 */
void deleteCurrentThread();

/**
 * 根据id寻找TCB
 * @retval nullptr 未找到
 */
TCB* findTCBById(id_t id);

/**
 * 使当前线程睡眠，等待唤醒
 */ 
void sleep();

/**
 * 唤醒某线程
 */ 
void wake(id_t thread_id);

#endif