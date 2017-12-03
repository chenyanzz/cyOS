#include "stdio.h"
#include "thread/thread.h"
extern "C"{


int start()
{
	init_terminal();
	printf("starting kernel..\n");
	while(1);
	return 0;
}


}