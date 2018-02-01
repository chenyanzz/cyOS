/**
 * Created by chenyan on 2018/1/30.
 */

#include "process/process.h"
#include "interrupt/irq.h"
#include "RTC_timer.h"
#include "RTC.h"
#include "time.h"
#include "stdio.h"

bool init_RTC_timer() {
    //开启irq8
    byte prevB = getRTCRegValue(StatusB);
    setRTCRegValue(StatusB, prevB | 0x40);

    set_RTC_timer(RTC_RATE);
    return true;
}

void set_RTC_timer(int rate) {
    byte prevA = getRTCRegValue(StatusA);
    setRTCRegValue(StatusA, (prevA & 0xF0) | rate); //freq是低4位
}

extern "C"
void RTC_timer_tick() {
    getRTCRegValue(StatusC);//激活下一次中断

//    printf("RTC\n");
    accept_new_irq(RTC_TIMER_IRQ);

    ///@note 注意！！！必须先接受下一次中断再调度，不然会死在线程里
    schedule();
}