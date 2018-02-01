#pragma once
/**
 * Created by chenyan on 2018/1/30.
 */


///频率64 =2^6 = 2^(16-(11-1))
#define RTC_RATE (11)

/**
 * 初始化
 */
bool init_RTC_timer();

/**
 * 设置时钟中断频率
 * @param rate 取值0~15,除0表示关闭外，n表示频率2^(16-(n-1))
 */
void set_RTC_timer(int rate);

/**
 * 处理时钟中断，递增系统时间
 */
extern "C" void RTC_timer_tick();