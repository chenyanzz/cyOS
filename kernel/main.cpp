#include "stdio.h"
using namespace std;
#include "thread/thread.h"
extern "C"{

/** 
 * @brief  cy
 * @note   32位主程序入口
 * @retval 不返回
 */
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