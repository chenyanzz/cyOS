/**
 * RTC 实时时钟相关函数
 * 获取当前时间
 */

#pragma once

#include "types.h"

struct time_t {
    byte second; ///<秒
    byte minute; ///<分
    byte hour;   ///<时
    byte day;    ///<一个月里第几天
    byte month;  ///<月
    int year;   ///<年
};

time_t getTime();

/**
 * 打印时间 YYYY mm:dd HH:MM:SS
 * @param time
 */
void printTime(time_t time);