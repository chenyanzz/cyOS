#pragma once
/**
 * Created by chenyan on 2018/1/30.
 */

#include "APIC_timer.h"
#include "RTC.h"

extern time_t currTime;
extern u32 ticks;

/**
 * 初始化当前时间
 * @return 是否成功
 */
bool init_time();

/**
 * 在时钟中断时调用，把时间递增
 */
void addTime();

/**
 * 检测各个时间单位是否溢出
 * 对于ms,second，minute，hour，day的溢出都会自动进位
 * 对于month的溢出会处理为年费进位，之后再按照新年分算平润年
 * @param time 时间结构体，修改后的数据也在这个结构体里
 */
void checkTimeOverFlow(time_t& time);