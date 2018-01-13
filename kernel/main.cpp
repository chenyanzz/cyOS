#include "asm.h"
#include "stdio.h"
#include "disk/fs.h"
#include "FGUI.h"
#include "stdlib.h"
#include "thread/thread.h"
#include "interrupt/idt.h"
#include "interrupt/timer.h"
#include "interrupt/keyboard.h"
#include "memory/page.h"

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


/** 
 * 32位主程序入口
 */
extern "C" void start()
{
	init(terminal);
	init(IDT);
	init(keyboard);
	init(mem_page);
	init(timer);
	// init(disk);
	// init(fs);


	while (true)
	{
		char c = read_charbuf();
		if((c!=0))
		{
			void* page = getFreePage();
			printf("%x\t",page);
		}
	}


	while(1);
}