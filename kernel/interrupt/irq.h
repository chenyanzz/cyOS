#pragma once

#include "types.h"

#define TIMER_IRQ (0)
#define KEYBOARD_IRQ (1)

/*关于int0x21
  ->某一位置一表示屏蔽此中断
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
u8 start_irq(u8 index);

/**
 * 关闭一个IRQ
 * @param index 那个irq号
 * @return 原先的irq寄存器
 */
u8 stop_irq(u8 index);

/**
 * 停止全部IRQ
 * @return 原先的irq寄存器
 */
u8 stop_all_irq();

/**
 * 开启全部使用中的IRQ
 * @return 原先的irq寄存器
 */
u8 start_all_irq();

/**
 * 还原irq状态
 * @param 要修改成的状态
 * @return 原先的irq寄存器
 */
u8 restore_irq(u8 mode);

/**
 * 开始接受下一次IRQ中断
 */
void accept_new_irq();


