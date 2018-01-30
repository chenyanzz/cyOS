//
// Created by chenyan on 2018/1/24.
//

#include "RTC.h"
#include "asm.h"
#include "stdio.h"

const word P_RTC_REG = 0x70;
const word P_RTC_DATA = 0x71;


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
};

byte getRTCRegValue(word reg) {
    /*
     1.将寄存器的编号(byte)写入P_RTC_REG端口
     2.从P_RTC_DATA端口读取数据(byte)
    */
    outb(P_RTC_REG, reg);
    return inb(P_RTC_DATA);
}

/// wait until the "Update in progress"(bit 7 of Status Register A) flag goes from "set" to "clear"
/// 等待正在刷新标志（SR.A）从1变0
void wait_RTC_update() {
    byte stat;

    //等待刷新标志位变成1
    do {
        stat = getRTCRegValue(StatusA);
    } while (bit(stat, 7) != 1);

    //等待刷新标志位恢复0
    do {
        stat = getRTCRegValue(StatusA);
    } while (bit(stat, 7) != 0);
}

time_t getTime() {
    time_t time = {0};

    wait_RTC_update();
    time.second = getRTCRegValue(Second);
    time.minute = getRTCRegValue(Minute);
    time.hour = getRTCRegValue(Hour);
    time.day = getRTCRegValue(Day);
    time.month = getRTCRegValue(Month);
    time.year = getRTCRegValue(Year);

    return time;
}

void printTime(time_t time) {
    printf("%4d %2d.%2d %2d:%2d:%2d",
           time.year, time.month, time.day, time.hour, time.minute, time.second);
}