#include "thread.h"
#include "stdio.h"
#include "asm.h"

struct TCB threads[MAX_THREADS];

void switch_to(TCB tcb)
{
    //压栈原来的寄存器
	push_reg();

	//存储栈顶指针
	asm("mov %0, ss":"=g"(tcb.ss));
	asm("mov %0, esp":"=g"(tcb.esp));

	//设置新堆栈帧
	asm("mov ss, %0"::"g"(tcb.ss));
	asm("mov esp, %0"::"g"(tcb.esp));

	//出栈新寄存器
	pop_reg();
}

void schedule()
{
	static id_count_t count_id;
}

id_t create_thread(TCB* ptcb,RUNNABLE proc)
{
	static id_t id=0;

	TCB& tcb = *ptcb;
	tcb.id = id++;
	tcb.ss = (SEGMENT) KNL_PROGRAM_SEG;
	tcb.esp = (void*) 0x10000;

	//切换到线程二的堆栈帧
	asm("mov esp, %0"::"g"(tcb.esp));  


	//压入主函数首地址
    asm("push %0"::"g"(proc));

	//proc函数开头要"push ebp"
    asm("push %0"::"g"(tcb.esp));

	asm(
        "pusha;"

		//压入各个段寄存器
		"push %0;"//ds
		"push %0;"//es
		"push %0;"//fs
		"push %0;"//gs

        "pushf;"
		::"g"(KNL_DATA_SEG)
	);

	//储存当前堆栈帧
	asm("mov %0, esp":"=g"(tcb.esp));

	return tcb.id;
}

extern void t1();
extern void t2();