#include "stdio.h"
#include "thread/thread.h"
#include "disk/fs.h"
#include "FGUI.h"

#define init(name) 	\
	if(init_##name())	\
	{	\
		setTerminalColor(WHITE);	\
		printf("init %s\t\t",#name);	\
		setTerminalColor(BLUE);	\
		printf("[succeed]\n");	\
	}else	\
	{	\
		setTerminalColor(WHITE);	\
		printf("init %s\t\t",#name);	\
		setTerminalColor(RED);	\
		printf("[failed]\n");	\
	}	\
	setTerminalColor(WHITE)


/** 
 * 32位主程序入口
 */
extern "C"

void start()
{
	init(terminal);
	init(disk);
	//format_disk();
	init(fs);

	// bootScreen();

	// FILE f = create_file("myXiaoYang.txt", data, sizeof(data));
	// sync();

	while(true);
}