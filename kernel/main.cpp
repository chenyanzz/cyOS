#include "stdio.h"
using namespace std;
#include "thread/thread.h"
#include "disk/basic_io.h"
/** 
 * @brief  cy
 * @note   32位主程序入口
 * @retval 不返回
 */

byte buf[1024];

extern "C"
int start()
{
	init_terminal();
	setTerminalColorAttr(0b00000110);
	disk_read(buf,4,{0,0,0});
	printHex(buf[0]);
	while(true);
	return 0;
}