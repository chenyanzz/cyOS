/**
 * RTC 实时时钟相关函数
 * 获取当前时间 & 时钟中断IRQ8
 */

#pragma once

#include "types.h"
#include "time.h"

extern const word P_RTC_REG;
extern const word P_RTC_DATA;


///RTC寄存器表
enum RTCReg {
    Second = 0x00,
    Minute = 0x02,
    Hour = 0x04,
    Weekday = 0x06,///<从没人改，自然也没得读
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


