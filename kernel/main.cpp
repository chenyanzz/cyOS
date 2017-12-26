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
		setTerminalColor(WHITE);      \
		printf("init %s\t\t", #name); \
		setTerminalColor(BLUE);       \
		printf("[succeed]\n");        \
	}                                 \
	else                              \
	{                                 \
		setTerminalColor(WHITE);      \
		printf("init %s\t\t", #name); \
		setTerminalColor(RED);        \
		printf("[failed]\n");         \
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
	//format_disk();
	// init(fs);
	asm("int 0x50");

	while (true)
	{
	}
}