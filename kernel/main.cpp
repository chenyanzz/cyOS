#include "stdio.h"
#include "thread/thread.h"
#include "disk/fs.h"
#include "FGUI.h"
#include "stdlib.h"
#include "idt.h"
#include "asm.h"
#include "timer.h"

#ifdef OS_DEBUG

#define init(name)                           	\
	if (init_##name())                       	\
	{                                        	\
		printf("${blue}[succeed]");        		\
		printf("${normal}init %s\n", #name); 	\
	}                                        	\
	else                                     	\
	{                                        	\
		printf("${red}[failed ]");          	\
		printf("${normal}init %s\n", #name); 	\
	}

#else //OS_DEBUG

#define init(name) init_##name()

#endif //OS_DEBUG


/** 
 * 32位主程序入口
 */
extern "C" void start()
{
	init(terminal);
	init(IDT);

	setCursor(0,0);
	for(int x=0;x<80;x++)
	{
		for(int y=0;y<24;y++)
		{
			printf("${bgblue} ");
		}
	}
	printf("${bgblack}                            ${white|bgblue}cyOS has failed!--HUAJI!!!");

	// init(timer);
	// init(disk);
	// init(fs);
	while (true)
	{
	}
}