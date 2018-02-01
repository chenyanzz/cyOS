//
// Created by chenyan on 2018/1/24.
//

#include "RTC.h"
#include "asm.h"
#include "stdio.h"


const word P_RTC_REG = 0x70;
const word P_RTC_DATA = 0x71;

byte getRTCRegValue(RTCReg reg) {
    /*
     1.将寄存器的编号(byte)写入P_RTC_REG端口
     2.从P_RTC_DATA端口读取数据(byte)
    */
    outb(P_RTC_REG, reg);
    return inb(P_RTC_DATA);
}

void setRTCRegValue(RTCReg reg, byte val) {
    /*
     1.将寄存器的编号(byte)写入P_RTC_REG端口
     2.从P_RTC_DATA端口读取数据(byte)
    */
    outb(P_RTC_REG, reg);
    outb(P_RTC_DATA, val);
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


time_t RTCgetTime() {
    time_t time = {0,0,0,0,0,0,0};

    byte registerB = getRTCRegValue(StatusB);

    wait_RTC_update();

    time.second = getRTCRegValue(Second);
    time.minute = getRTCRegValue(Minute);
    time.hour = getRTCRegValue(Hour);
    time.day = getRTCRegValue(Day);
    time.month = getRTCRegValue(Month);
    time.year = getRTCRegValue(Year);

    // Convert BCD to binary values if necessary

    if (!(registerB & 0x04)) {
        time.second = (time.second & 0x0F) + ((time.second / 16) * 10);
        time.minute = (time.minute & 0x0F) + ((time.minute / 16) * 10);
        time.hour = ((time.hour & 0x0F) + (((time.hour & 0x70) / 16) * 10)) | (time.hour & 0x80);
        time.day = (time.day & 0x0F) + ((time.day / 16) * 10);
        time.month = (time.month & 0x0F) + ((time.month / 16) * 10);
        time.year = (time.year & 0x0F) + ((time.year / 16) * 10)+2000;
    }

    // Convert 12 hour clock to 24 hour clock if necessary

    if (!(registerB & 0x02) && (time.hour & 0x80)) {
        time.hour = ((time.hour & 0x7F) + 12) % 24;
    }

    return time;
}


void printTime(time_t time) {
    printf("%4d %2d.%2d %2d:%2d:%2d\n",
           time.year, time.month, time.day, time.hour, time.minute, time.second);
}

