#pragma once
/**
 * Created by chenyan on 2018/1/30.
 */

#include "APIC_timer.h"
#include "types.h"

struct time_t {
    int ms;     ///<毫秒 @note byte会溢出
    int second; ///<秒
    int minute; ///<分
    int hour;   ///<时
    int day;    ///<一个月里第几天
    int month;  ///<月
    int year;   ///<年
};

extern time_t currTime;
extern u32 ticks;

/**
 * 判断两个时间是否相等
 * @return 是否相等
 */
bool operator==(time_t t1,time_t t2);

/**
 * ...无fuck说
 * @return !(t1=t2)
 * @see bool operator==(time_t t1,time_t t2);
 */
bool operator!=(time_t t1,time_t t2);

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
 * 判断一年是不是闰年
 * @param year 年份
 * @return 是不是闰年
 */
bool isLeapYear(int year);

/**
 * 一个月有几天
 * @param month 月份
 * @param year 年份
 * @return 几天
 */
int daysInMonth(int month,int year);

/**
 * 检测各个时间单位是否溢出
 * 对于ms,second，minute，hour，day的溢出都会自动进位
 * 对于month的溢出会处理为年费进位，之后再按照新年分算平润年
 * @param time 时间结构体，修改后的数据也在这个结构体里
 */
void checkTimeOverFlow(time_t& time);

/**
 * 打印时间 YYYY mm:dd HH:MM:SS::ms
 * @param time
 */
void printTime(time_t time);