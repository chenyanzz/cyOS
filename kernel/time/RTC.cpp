//
// Created by chenyan on 2018/1/24.
//

#include "RTC.h"
#include "asm.h"
#include "stdio.h"
#include "time.h"

const word P_RTC_REG = 0x70;
const word P_RTC_DATA = 0x71;

static bool isBCDMode;

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

bool get_update_in_progress_flag() {
    byte stat = getRTCRegValue(StatusA);
    return bit(stat, 7);
}

/// wait until the "Update in progress"(bit 7 of Status Register A) flag goes from "set" to "clear"
/// 等待正在刷新标志（SR.A）变0
void wait_RTC_update() {
    byte stat;

    //等待刷新标志位恢复0
    do {
        stat = getRTCRegValue(StatusA);
    } while (bit(stat, 7) != 0);
}

static byte registerB;

/**
 * 获取RTC里的数据，仅此而已
 * @param time时间结构体
 */
void getTmpTime(time_t &time) {
    byte registerB = getRTCRegValue(StatusB);
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
        time.year = (time.year & 0x0F) + ((time.year / 16) * 10) + 2000;
    }

    // Convert 12 hour clock to 24 hour clock if necessary

    if (!(registerB & 0x02) && (time.hour & 0x80)) {
        time.hour = ((time.hour & 0x7F) + 12) % 24;
    }
}

time_t RTCgetTime() {

    time_t last_time;

    // Note: This uses the "read registers until you get the same values twice in a row" technique
    //       to avoid getting dodgy/inconsistent values due to RTC updates

    while (get_update_in_progress_flag());                // Make sure an update isn't in progress
    getTmpTime(currTime);

    do {
        last_time = currTime;
        while (get_update_in_progress_flag());           // Make sure an update isn't in progress
        getTmpTime(currTime);
    } while (last_time != currTime);


    time_t time;
    getTmpTime(time);

    return time;

}

