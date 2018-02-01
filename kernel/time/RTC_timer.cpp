/**
 * Created by chenyan on 2018/1/30.
 */

#include "thread/thread.h"
#include "interrupt/irq.h"
#include "RTC_timer.h"
#include "RTC.h"
#include "time.h"

bool init_RTC_timer()
{
    set_RTC_timer(RTC_RATE);
    return true;
}

void set_RTC_timer(int rate)
{
    char prev=getRTCRegValue(StatusA);
    setRTCRegValue(StatusA, (prev & 0xF0) | rate); //freq是低4位
}

extern "C"
void RTC_timer_tick()
{
    getRTCRegValue(StatusC);

    accept_new_irq(RTC_TIMER_IRQ);

    ///@note 注意！！！必须先接受下一次中断再调度，不然会死在线程里
    schedule();
}