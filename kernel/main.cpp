#include "asm.h"
#include "stdio.h"
#include "disk/fs.h"
#include "FGUI.h"
#include "stdlib.h"
#include "thread/thread.h"
#include "interrupt/idt.h"
#include "interrupt/irq.h"
#include "interrupt/timer.h"
#include "interrupt/keyboard.h"
#include "memory/page.h"
#include "thread/thread.h"

#ifndef OS_DEBUG
#define init(name) init_##name()
#else
#define init(name)                           	\
	if (init_##name())                       	\
	{                                        	\
		printf("${blue}[succeed]");        		\
		printf("${normal}init %s\n", #name); 	\
	}                                        	\
	else                                     	\
	{                                        	\
		printf("${red}[failed!]");          	\
		printf("${normal}init %s\n", #name); 	\
	}
#endif //OS_DEBUG

//为了没线程后schedule飞走，加的空循环
void nullLoop();

void t1();
void t2();

/**
 * 32位主程序入口
 * 注意：此函数必须是main.cpp的第一个函数
 */
extern "C" void start()
{
	//各种初始化
	init(terminal);
	init(IDT);
	init(keyboard);
	init(mem_page);
	// init(timer);
	init(thread);

	// init(disk);
	// init(fs);

	create_thread(nullLoop,"nullLoop");
	create_thread(t1,"t1");
	create_thread(t2,"t2");


	//开启中断
	sti();
	start_all_irq();

	
}

void nullLoop()
{
	while(true);
}

void t1()
{
		printf("a");		
}

void t2()
{
		printf("b");
}