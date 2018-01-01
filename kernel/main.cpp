#include "asm.h"
#include "stdio.h"
#include "disk/fs.h"
#include "FGUI.h"
#include "stdlib.h"
#include "thread/thread.h"
#include "interrupt/idt.h"
#include "interrupt/timer.h"
#include "interrupt/keyboard.h"

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
	init(timer);
	init(keyboard);
	// init(disk);
	// init(fs);
	printf("%d %f %c %s %x\n",12345,(double)1.2345,'z',"yyyyy",222);
	while(1);
	// while (true)
	// {
	// 	char c = read_charbuf();
	// 	if((c!=0))
	// 	{
	// 		printChar(c);
	// 	}
	// }
}