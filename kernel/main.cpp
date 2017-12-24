#include "stdio.h"
#include "thread/thread.h"
#include "disk/fs.h"
#include "FGUI.h"

#define init(name) 	\
	setTerminalColor(WHITE);	\
	printf("init %s\t\t",#name);	\
	if(init_##name())	\
	{	\
		setTerminalColor(BLUE);	\
		printf("[succeed]\n");	\
	}else	\
	{	\
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
	init_terminal();
	bootScreen();

	init(disk);
	format_disk();
	init(fs);
	// cls();
	// char data[] = "Xiao Yang Handsome";

	// int i=0;
	// switch(i)
	// {
	// case 0:
	// printInt(0);
	// break;
	// case 1:
	// printInt(1);
	// }
	
	printf("%d",10231);

	// FILE f = create_file("myXiaoYang.txt", data, sizeof(data));
	// sync();

	while(true);
}