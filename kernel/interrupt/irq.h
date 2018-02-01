#pragma once

#include "types.h"

#define APIC_TIMER_IRQ (0)
#define PIT_TIMER_IRQ (0)
#define KEYBOARD_IRQ (1)
#define SLAVE_IRQ (2)
#define RTC_TIMER_IRQ (8)

/**
 * 关于P_PIC_ -> 某一位置一表示屏蔽此中断
 * 返回的u16是master<<8+slave
*/



/**
 * 初始化IRQ
 */
void setup_irq();

/**
 * 开启一个IRQ
 * @param index 那个irq号
 * @return 原先的irq寄存器
 */
u16 start_irq(u8 index);

/**
 * 关闭一个IRQ
 * @param index 那个irq号
 * @return 原先的irq寄存器
 */
u16 stop_irq(u8 index);

/**
 * 停止全部IRQ
 * @return 原先的irq寄存器
 */
u16 stop_all_irq();

/**
 * 开启全部使用中的IRQ
 * @return 原先的irq寄存器
 */
u16 start_all_irq();

/**
 * 还原irq状态
 * @param 要修改成的状态
 * @return 原先的irq寄存器
 */
u16 restore_irq(u16 mode);

/**
 * 开始接受下一次IRQ中断
 * @param irq IRQ号
 */
void accept_new_irq(int irq);


