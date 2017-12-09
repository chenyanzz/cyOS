#include "thread.h"
#include "stdio.h"

struct TCB tcb1, tcb2;

void switch_to_t2()
{
    //压栈原来的寄存器
	asm(
        "pusha;"
        "push ebp;"
        "push ds;"
        "push es;"
        "push fs;"
        "push gs;"
        "pushf;"
    );

	//存储栈顶指针
	asm("mov %0, ss":"=g"(tcb1.ss));
	asm("mov %0, esp":"=g"(tcb1.esp));

	//设置新堆栈帧
	asm("mov ss, %0"::"g"(tcb2.ss));
	asm("mov esp, %0"::"g"(tcb2.esp));

	//出栈新寄存器
	asm(
		"popf;"
		"pop gs;"
		"pop fs;"
		"pop es;"
		"pop ds;"
		"pop ebp;"
		"popa;"
    );
}

void switch_to_t1()
{
	//压栈原来的寄存器
	asm(
		"pusha;"
		"push ebp;"
		"push ds;"
		"push es;"
		"push fs;"
		"push gs;"
		"pushf;"
    );
    
	//存储栈顶指针
	asm("mov %0, ss":"=g"(tcb2.ss));
	asm("mov %0, esp":"=g"(tcb2.esp));


	//设置新堆栈帧
	asm("mov ss, %0"::"g"(tcb1.ss));
    asm("mov esp, %0"::"g"(tcb1.esp));

	//弹栈新寄存器
	asm(
		"popf;"
		"pop gs;"
		"pop fs;"
		"pop es;"
		"pop ds;"
		"pop ebp;"
		"popa;"
	);
}

extern void t1();
extern void t2();

void test_thread()
{
	tcb1.id = 1;
	tcb1.ss = (SEGMENT) 0x10;
	tcb1.esp = (MEM_ADDR) 0x10000;

	tcb2.id = 2;
	tcb2.ss = (SEGMENT) 0x10;
	tcb2.esp = (MEM_ADDR) 0x20000;

	//切换到线程二的堆栈帧
	asm("mov esp, %0"::"g"(tcb2.esp));  

	RUNNABLE r2 = &t2;

	//压入线程二的首地址
    asm("push %0"::"g"(r2));

    asm("push %0"::"g"(tcb2.esp));

	asm(
        "pusha;"
		"push ebp;"
		"push ds;"
		"push es;"
		"push fs;"
		"push gs;"
        "pushf;"
	);

	//储存当前堆栈帧为开始状态
	asm("mov %0, esp":"=g"(tcb2.esp));

	//切换到t1堆栈帧
	asm("mov esp, %0"::"g"(tcb1.esp));

	t1();
}
