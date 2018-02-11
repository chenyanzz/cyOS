#include <kernel/time/time.h>
#include <kernel/time/PIT_timer.h>
#include <kernel/time/RTC_timer.h>
#include "asm.h"
#include "stdio.h"
#include "disk/fs.h"
#include "FGUI.h"
#include "stdlib.h"
#include "process/process.h"
#include "interrupt/idt.h"
#include "interrupt/irq.h"
#include "kernel/time/APIC_timer.h"
#include "interrupt/keyboard.h"
#include "memory/page.h"
#include "process/process.h"
#include "gdt.h"
#include "time/RTC.h"
#include "init.h"

/**
 * 为了防止没线程后schedule飞走，加的空循环
 */
void nullLoop();

void t1();

void t2();

/**
 * kernel主程序入口
 */
extern "C" void kernel_main() {
    cli();
    stop_all_irq();

    //不先初始化输出后面stdio就崩
    init_terminal();
    init(GDT);
    init(IDT);

//    init(keyboard);
//    init(time);//系统时间
//    init(RTC_timer);//任务调度中断
//    init(PIT_timer);//时钟tick中断
//    init(process);

    // init(disk);
    // init(fs);

//    create_process(nullLoop, "nullLoop");
//    create_process(t1, "t1");
//    create_process(t2, "t2");

    //开启中断
//    sti();
//    start_all_irq();


    while (true){}///@note 这里一定要死循环
}

void nullLoop() {
    while (true) {
    }
}

void t1() {
//    printf("t1 run\n");
}

void t2() {
//    printf("t2 run\n");
}
