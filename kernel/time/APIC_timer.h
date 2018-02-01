/**
 * APIC timer - IRQ0
 * 废了废了，不靠谱，PIT_timer多棒棒
 */


#pragma once

#define APIC_TIMER_TICKS_PER_SECOND 50

/**
 * 初始化
 */
bool init_APIC_timer();

/**
 * 设置时钟频率
 * @param freq 每几次内部tick一次中断信号
 */
void set_APIC_timer(int freq);

/**
 * 处理时钟中断，切换线程
 */
extern "C" void APIC_timer_tick();


