#include "asm.h"
#include "stdio.h"
#include "disk/fs.h"
#include "FGUI.h"
#include "stdlib.h"
#include "thread/thread.h"
#include "interrupt/idt.h"
#include "interrupt/irq.h"
#include "kernel/time/timer.h"
#include "interrupt/keyboard.h"
#include "memory/page.h"
#include "thread/thread.h"
#include "gdt.h"

/**
 * 初始化用的宏，用来打印是否成功
 */
#ifndef OS_DEBUG
#define init(name) init_##name()
#else
#define init(name)                            \
    if (init_##name())                        \
    {                                            \
        printf("${blue}[succeed]");                \
        printf("${normal}init %s\n", #name);    \
    }                                            \
    else                                        \
    {                                            \
        printf("${red}[failed!]");            \
        printf("${normal}init %s\n", #name);    \
    }
#endif

/**
 * 为了防止没线程后schedule飞走，加的空循环
 */
void nullLoop();

void t1();

void t2();

/**
 * 32位主程序入口
 * 注意：此函数必须是main.cpp的第一个函数
 */
extern "C" void start() {

    init(terminal);
    init(IDT);
    //要先初始化内存再初始化GDT
    init(mem_page);
    init(GDT);
    init(keyboard);
    init(timer);
    init(thread);

    // init(disk);
    // init(fs);

    create_thread(nullLoop, "nullLoop");
    create_thread(t1, "t1");
    create_thread(t2, "t2");

    //开启中断
    start_all_irq();
    sti();

}

void nullLoop() {
    while (true) {
        //延时一会
        for (int i = 0; i < 0xFFFFF; i++) {

        }
        printChar('.');
    }
}

void t1() {
    printf("t1 run\n");
}

void t2() {
    printf("t2 run\n");
}
