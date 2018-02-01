/**
 * RTC 实时时钟相关函数
 * 获取当前时间 & 时钟中断IRQ8
 */

#pragma once

#include "types.h"

struct time_t {
    byte ms;    ///<毫秒
    byte second; ///<秒
    byte minute; ///<分
    byte hour;   ///<时
    byte day;    ///<一个月里第几天
    byte month;  ///<月
    int year;   ///<年
};

extern const word P_RTC_REG;
extern const word P_RTC_DATA;


///RTC寄存器表
enum RTCReg {
    Second = 0x00,
    Minute = 0x02,
    Hour = 0x04,
    Weekday = 0x06,
    Day = 0x07,
    Month = 0x08,
    Year = 0x09,
    StatusA = 0x0A,
    StatusB = 0x0B,
    StatusC = 0x0C,
};

/**
 * 读RTC寄存器里的值
 * @param reg 寄存器编号
 * @return 值
 */
byte getRTCRegValue(RTCReg reg);

/**
 * 写RTC寄存器
 * @param reg 寄存器编号
 * @param val 值
 */
void setRTCRegValue(RTCReg reg, byte val);

/**
 * 获取RTC中的时间
 * @note 这个函数非常耗费时间，应该只在启动系统时读一次
 * @return 时间结构体
 */
time_t RTCgetTime();

/**
 * 打印时间 YYYY mm:dd HH:MM:SS
 * @param time
 */
void printTime(time_t time);


