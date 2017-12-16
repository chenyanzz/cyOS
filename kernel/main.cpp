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

	disk_read(buf,25,{0,0,0});
	printf(buf);
	while(true);
	return 0;
}