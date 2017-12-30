#include "stdio.h"
#include "thread/thread.h"
#include "disk/fs.h"
#include "FGUI.h"
#include "stdlib.h"
#include "idt.h"
#include "asm.h"
#include "timer.h"

#ifdef OS_DEBUG

#define init(name)                           \
	if (init_##name())                       \
	{                                        \
		printf("${blue}[succeed]${normal}"); \
		printf("init %s\n", #name);          \
	}                                        \
	else                                     \
	{                                        \
		printf("${red}[failed]${normal}");   \
		printf("init %s\n", #name);          \
	}                                        \
	setTerminalColor(WHITE)

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
	// init(disk);
	// init(fs);
	while (true)
	{
	}
}