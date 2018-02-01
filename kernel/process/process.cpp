#include "process.h"
#include "stdio.h"
#include "asm.h"
#include "stdlib.h"
#include "interrupt/irq.h"
#include "memory/page.h"


PCB processes[MAX_PROCESSES];
PCB PCB_kernel;///< 为了带一次调度的时候正常
PCB *current = &PCB_kernel;///< 为了带一次调度的时候正常

int count_processes;

//检测tcb是否可被调度 存在&没等待&未结束
#define isSwitchable(pcb) ((pcb.isPresent) && (!pcb.isWaiting) && (!pcb.isEnd))

bool init_process()
{
	memset(processes, 0, sizeof(processes));
	count_processes=0;
	return true;
}

void switch_to(PCB *ppcb)
{
	//压栈原来的寄存器
	push_reg();

	//存储栈顶指针
	asm("mov %0, ss"
		: "=g"(current->ss));
	asm("mov %0, esp"
		: "=g"(current->esp));

	//设置当前线程为新线程
	current = ppcb;

	//设置新堆栈帧
	asm("mov ss, %0" ::"g"(ppcb->ss));
	asm("mov esp, %0" ::"g"(ppcb->esp));

	//出栈新寄存器
	pop_reg();
}

void schedule()
{
	//如果没有线程就直接返回
	if(count_processes==0)return;

	static id_t pos = -1;
	//每次从上一次的下一个ID开始搜索
	pos++;
	id_t oldPos = pos;

	//FIFO方式获取下一个线程
	while (!isSwitchable(processes[pos]))
	{

		/*这里！！！必须先++在判断*/
		pos++;

		if (pos == MAX_PROCESSES)
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
	PCB *tcb = &processes[pos];
	switch_to(tcb);
}

id_t create_process(PROCESS_MAIN_PROC proc, char *name, bool bWait)
{
	static id_t id = 1;

	PCB *ptcb = findNewPCB();
	ptcb->id = id++;
	ptcb->ss = (u32)KNL_DATA_SEG;
	ptcb->esp = ((u32)allocPage()) + SIZE_MEM_PAGE - 1;

	ptcb->isPresent = true;
	ptcb->isEnd = false;
	ptcb->isWaiting = bWait;

	strcpy(ptcb->name, name);

	//切换到线程二的堆栈帧
	asm("mov esp, %0" ::"g"(ptcb->esp));

	//压入线程最后ret时的返回地址
	asm("push %0" ::"g"((u32)deleteCurrentProcess));

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

	close_int();
	count_processes++;
	start_int();

	return ptcb->id;
}

PCB *findNewPCB()
{
	for (id_t i = 0; i < MAX_PROCESSES; i++)
	{
		if (!processes[i].isPresent)
		{
			return &processes[i];
		}
	}
	return nullptr;
}

void deleteCurrentProcess()
{
	close_int();
	
	printf("%s ended\n",current->name);
	current->isEnd=true;
	current->isPresent = false;
	current->id=0;

	count_processes--;

    start_int();
	schedule();

}

void exit(int retCode)
{
	deleteCurrentProcess();
}

PCB* findPCBById(id_t id)
{
	id_t i=0;
	for(;i<MAX_PROCESSES;i++)
	{
		if(processes[i].id==id)break;
	}
	if(i==MAX_PROCESSES)return nullptr;

	PCB* ptcb = &processes[i];
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
	PCB* ptcb = findPCBById(thread_id);
	if(!ptcb)return;
	ptcb->isWaiting=false;
	schedule();
}