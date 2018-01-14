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

extern void t1();
extern void t2();

/** 
 * 32位主程序入口
 * 注意：此函数必须是main.cpp的第一个函数
 */
extern "C" void start()
{
	init(terminal);
	init(IDT);
	init(keyboard);
	init(mem_page);
	init(timer);
	init(thread);
	// init(disk);
	// init(fs);

	create_thread(t1,"t1");
	create_thread(t2,"t2");

	start_all_irq();

	while(1);
}

void t1()
{
	while (1)
	{
		for (int i = 0; i < 7; i++)
			printChar('1');
	}
}

void t2()
{
	while (1)
	{
		for (int i = 0; i < 7; i++)
			printChar('2');
	}
}