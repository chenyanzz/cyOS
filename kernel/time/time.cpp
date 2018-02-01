/**
 * Created by chenyan on 2018/1/30.
 */

#include "time.h"
#include "PIT_timer.h"

time_t currTime;

u32 ticks;

bool init_time()
{
    currTime = RTCgetTime();
    return true;
}

bool isLeapYear(int year) {
    /*
    普通年（不能被100整除的年份）能被4整除的为闰年
    世纪年（能被100整除的年份）能被400整除的是闰年
    */
    return ((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0);
}

void addTime() {
    currTime.ms+=PIT_TICK_MS;
    if(currTime.ms>=1000) {
        checkTimeOverFlow(currTime);
    }
}

void add_second() {
    currTime.second++;
    checkTimeOverFlow(currTime);
}

int daysInMonth(int month,int year)
{
    //每月有几天，从1开始下标，与月份相同
    int days[]={0,31,28,31,30,31,30,31,31,30,31,30,31};
    if(isLeapYear(year))days[2]++;
    return days[month];
}

void checkTimeOverFlow(time_t& time) {
    time.second += time.ms / 1000;
    time.ms %= 1000;

    time.minute += time.second / 60;
    time.second %= 60;

    time.hour += time.minute / 60;
    time.minute %= 60;

    time.day += time.hour / 24;
    time.hour %= 24;

    time.year += time.month / 12;
    time.hour %= 12;

    while(time.day>daysInMonth(time.month,time.year))
    {
        time.day-=daysInMonth(time.month,time.year);
        time.month++;
        if(time.month==13){
            time.month=1;
            time.year++;
        }
    }
}