/**
 * Created by chenyan on 2018/1/30.
 */

#include "PIT_timer.h"
#include "printf.h"
#include "asm.h"
#include "types.h"
#include "kernel/interrupt/irq.h"
#include "time.h"


bool init_PIT_timer() {
    ticks = 0;
    set_PIT_timer();
    return true;
}

void set_PIT_timer() {
/*
 寄存器传入 = 3344,
 tick间隔 = 3.99ms
 每100次tick修正一次（ms++）
*/
    outb(P_PIT_COMMAND,0b00110110);//Channel 0, 2bytes data, mode 3, binary data
    outb(P_PIT_DATA,(byte)3344);
    outb(P_PIT_DATA,(byte)3344>>8);
}

extern "C"
void PIT_timer_tick() {
//    printChar('P');
    close_int();
    ticks++;
    if (ticks % 100 == 0) {
        currTime.ms++;
    }
    addTime();
    start_int();
    accept_new_irq(PIT_TIMER_IRQ);
}