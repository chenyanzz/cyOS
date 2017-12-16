#include "stdio.h"
using namespace std;
#include "thread/thread.h"
#include "disk/basic_io.h"
/** 
 * @brief  cy
 * @note   32位主程序入口
 * @retval 不返回
 */


extern "C"
int start()
{
	char buf[1024] = "Hia Hia Hia -#_#- Xiao Yang Handsome\n\0";
	init_terminal();

	// disk_read(buf,25,{0,0,0});
	//printf(buf);
	disk_write(buf,2,{0,0,0});
	printf("wrote.\n");
	while(true);
	return 0;
}