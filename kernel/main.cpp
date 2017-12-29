#define OS_DEBUG

#include "stdio.h"
#include "thread/thread.h"
#include "disk/fs.h"
#include "FGUI.h"
#include "stdlib.h"
#include "idt.h"
#include "asm.h"
#ifdef OS_DEBUG

#define init(name)                    \
	if (init_##name())                \
	{                                 \
		printf("init %s\t\t", #name); \
		printf("${blue}[succeed]\n"); \
	}                                 \
	else                              \
	{                                 \
		printf("init %s\t\t", #name); \
		printf("${red}[failed]\n");   \
	}                                 \
	setTerminalColor(WHITE)

#else
#define init(name) init_##name()
#endif //OS_DEBUG

/** 
 * 32位主程序入口
 */
extern "C" void start()
{
	init(terminal);
	init(IDT);
	// init(disk);
	// init(fs);
	printf("will int\n");
	asm("int 0");
	printf("int end!\n");
	while (true)
	{
	}
}