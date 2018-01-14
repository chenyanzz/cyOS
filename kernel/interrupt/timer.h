#ifndef TIMER_H
#define TIMER_H

#define TICK_PER_SECOND 100

/**
 * 初始化
 */
bool init_timer();

/**
 * 设置时钟频率
 * @param freq 每几次内部tick一次中断信号
 */
void set_timer(int freq);

/**
 * 处理时钟中断，切换线程
 */
extern "C" void timer_tick();

#endif //TIMER_H
