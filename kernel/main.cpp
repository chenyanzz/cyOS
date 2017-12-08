#include "stdio.h"
#include "thread/thread.h"
extern "C"{


int start()
{
	init_terminal();
	printf("starting kernel..\n");
	setTerminalColorAttr(0b00000110);
	printf("--Helloworld!--\n");
	while(1);
	return 0;
}


}