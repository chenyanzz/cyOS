#include "stdio.h"
#include "timer.h"

#ifdef OS_DEBUG

#define COMMON_HANDLE_INT(index) printf("${YELLOW}int%d called!\n", index)

#else //OS_DEBUG

#define COMMON_HANDLE_INT(index)

#endif


#define FATAL(msg) printf("${RED}FATAL:%s\nPlease reboot!!!\n", msg)

extern "C" {

/**
 * int 13 一般性保护异常
 * 红字报错，死循环
 */
void general_protection_fault_handler()
{
	COMMON_HANDLE_INT(13);
	FATAL("general_protection_fault");

	while (true)
	{
	}
}

/**
 * int 0 除0错
 * 处理方法：红字报错，死循环
 */
void devide_zero_handler()
{
	COMMON_HANDLE_INT(0);
	FATAL("devided by zero");

	while (true)
	{
	}
}

extern void timer_tick();

extern void key_state_changed();

} //extern "C"