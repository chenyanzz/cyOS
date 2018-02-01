#pragma once
/**
 * Created by chenyan on 2018/1/30.
 */

#define PIT_TICK_MS 4   ///< 两次tick的间隔

#define P_PIT_DATA (0x40)
#define P_PIT_COMMAND (0x43)

/**
 * 初始化
 */
bool init_PIT_timer();

/**
 * 设置时钟频率 每3.99ms一次tick
 */
void set_PIT_timer();

/**
 * 处理时钟中断，计时
 */
extern "C" void PIT_timer_tick();

