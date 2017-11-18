#include "stdio.h"
#include "thread/thread.h"

void _start()
{
	init_terminal();
	printf("starting kernel..\n");
	while(1);
}