#include "stdio.h"
#include "kernel/time/APIC_timer.h"
#include "asm.h"

///通用处理中断的代码

#ifdef OS_DEBUG

#else //OS_DEBUG

#define COMMON_HANDLE_INT(index)

#endif

///致命错误的报错
#define FATAL(msg) printf("${RED}FATAL:"#msg"\nPlease reboot!!!\n")

extern "C" {

/**
 * int 13 一般性保护异常
 * 处理方法：红字报错，死循环
 */
void general_protection_fault_handler()
{
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
	FATAL("devided by zero");

	while (true)
	{
	}
}

/**
 * int 其他的 不明中断
 * 处理方法：红字报错，死循环
 */
void unexpected_int_handler()
{
	FATAL("unexpected interrupt");

	while (true)
	{
	}
}

extern void PIT_timer_tick();

extern void key_state_changed();

extern void RTC_timer_tick();

} //extern "C"