#include "thread.h"
#include "stdio.h"
#include "asm.h"
#include "stdlib.h"
#include "interrupt/irq.h"


TCB threads[MAX_THREADS];

TCB TCB_kernel;
TCB *current = &TCB_kernel;

//检测tcb是否可被调度 存在&没等待&未结束
#define isSwitchable(tcb) ((tcb.isPresent) && (!tcb.isWaiting) && (!tcb.isEnd))

bool init_thread()
{
	memset(threads, 0, sizeof(threads));
	return true;
}

void switch_to(TCB *ptcb)
{
	//压栈原来的寄存器
	push_reg();

	//存储栈顶指针
	asm("mov %0, ss"
		: "=g"(current->ss));
	asm("mov %0, esp"
		: "=g"(current->esp));

	//设置当前线程为新线程
	current = ptcb;

	//设置新堆栈帧
	asm("mov ss, %0" ::"g"(ptcb->ss));
	asm("mov esp, %0" ::"g"(ptcb->esp));

	//出栈新寄存器
	pop_reg();
}

void schedule()
{
	static id_t pos = -1;
	//每次从上一次的下一个ID开始搜索
	pos++;
	id_t oldPos = pos;

	//FIFO方式获取下一个线程
	while (!isSwitchable(threads[pos]))
	{

		/*这里！！！必须先++在判断*/
		pos++;

		if (pos == MAX_THREADS)
		{
			pos = 0;
			continue;
		}
		
		//如果循环了一圈没有线程就退出
		if (pos == oldPos)
		{
			return;
		}
	}

	//如果只有一个线程，就不切换
	//那也还是切换一下好了

	//切换到新线程
	TCB *tcb = &threads[pos];
	switch_to(tcb);
}

id_t create_thread(RUNNABLE proc, char *name, bool bWait)
{
	static id_t id = 1;

	TCB *ptcb = findNewTCB();
	ptcb->id = id++;
	ptcb->ss = (SEGMENT)KNL_DATA_SEG;
	ptcb->esp = allocPage() + SIZE_MEM_PAGE - 1;

	ptcb->isPresent = true;
	ptcb->isEnd = false;
	ptcb->isWaiting = bWait;

	strcpy(ptcb->name, name);

	//切换到线程二的堆栈帧
	asm("mov esp, %0" ::"g"(ptcb->esp));

	//压入线程最后ret时的返回地址
	asm("push %0" ::"g"((u32)deleteCurrentThread));

	//压入主函数首地址
	asm("push %0" ::"g"(proc));

	//给切换时switch_to函数最后的leave指令压入esp
	asm("push %0" ::"g"(ptcb->esp));

	asm(
		"pushad;"

		//压入各个段寄存器
		"push %0;" //ds
		"push %0;" //es
		"push %0;" //fs
		"push %0;" //gs

		"pushf;" ::"g"(KNL_DATA_SEG));

	//储存当前堆栈帧
	asm("mov %0, esp"
		: "=g"(ptcb->esp));

	return ptcb->id;
}

TCB *findNewTCB()
{
	for (id_t i = 0; i < MAX_THREADS; i++)
	{
		if (!threads[i].isPresent)
		{
			return &threads[i];
		}
	}
	return nullptr;
}

void deleteCurrentThread()
{
	close_int();
	
	printf("%s ended\n",current->name);
	current->isEnd=true;
	current->isPresent = false;
	current->id=0;
	schedule();

	start_int();
}

void exit(int retCode)
{
	deleteCurrentThread();
}

TCB* findTCBById(id_t id)
{
	id_t i=0;
	for(;i<MAX_THREADS;i++)
	{
		if(threads[i].id==id)break;
	}
	if(i==MAX_THREADS)return nullptr;

	TCB* ptcb = &threads[i];
	if(ptcb->isPresent&&!ptcb->isEnd)
	{
		return ptcb;
	}
	return nullptr;
}

void sleep()
{
	current->isWaiting=true;
	schedule();
}

void wake(id_t thread_id)
{
	TCB* ptcb = findTCBById(thread_id);
	if(!ptcb)return;
	ptcb->isWaiting=false;
	schedule();
}