#include "stdio.h"
#include "thread/thread.h"

int main()
{
	init_terminal();
	printf("starting kernel..\n");
	while(1);
	return 0;
}