#include "irq.h"
#include "irq.h"
#include "asm.h"
#include "deal_int.h"
#include "idt.h"

#define IRQ(n) (0x20 + (n))


#define P_PIC_MASTER (0x21)///< 控制是否接受某中断
#define P_PIC_SLAVE (0xA1)///< 控制是否接受某中断

#define P_PIC_MASTER_COMMAND    (0x20)
#define P_PIC_SLAVE_COMMAND    (0xA0)

#define PIC_EOI        0x20        /* End-of-interrupt command code */

void setup_irq() {
    stop_all_irq();

//	set_gate(IRQ(APIC_TIMER_IRQ),deal_irq_0,INT);//ACPI时钟
    set_gate(IRQ(PIT_TIMER_IRQ), deal_irq_0, INT);//PIT时钟
    set_gate(IRQ(RTC_TIMER_IRQ), deal_irq_8, INT);//RTC实时时钟
    set_gate(IRQ(KEYBOARD_IRQ), deal_irq_1, INT);//键盘
}

u16 start_irq(u8 index) {
    u8 oldst_m = inb(P_PIC_MASTER);
    u8 oldst_s = inb(P_PIC_SLAVE);
    if (index < 8) {
        u8 st = oldst_m;
        clrb(st, index);
        outb(P_PIC_MASTER, st);
    } else {
        u8 st = oldst_s;
        clrb(st, index - 8);
        outb(P_PIC_SLAVE, st);
    }

    return (oldst_m << 8) + oldst_s;

}

u16 stop_irq(u8 index) {
    u8 oldst_m = inb(P_PIC_MASTER);
    u8 oldst_s = inb(P_PIC_SLAVE);
    if (index < 8) {
        u8 st = oldst_m;
        setb(st, index);
        outb(P_PIC_MASTER, st);
    } else {
        u8 st = oldst_s;
        setb(st, index - 8);
        outb(P_PIC_SLAVE, st);
    }

    return (oldst_m << 8) + oldst_s;
}

u16 stop_all_irq() {
    u8 oldst_m = inb(P_PIC_MASTER);
    u8 oldst_s = inb(P_PIC_SLAVE);
    outb(P_PIC_MASTER, 0b11111011);
    outb(P_PIC_SLAVE, 0b11111111);
    return (oldst_m << 8) + oldst_s;
}

u16 restore_irq(u16 mode) {

    u8 oldst_m = inb(P_PIC_MASTER);
    u8 oldst_s = inb(P_PIC_SLAVE);
    outb(P_PIC_MASTER, (byte) mode >> 8);
    outb(P_PIC_SLAVE, (byte) mode);
    return (oldst_m << 8) + oldst_s;
}

void accept_new_irq(int irq) {
    if(irq>=8) {
        outb(P_PIC_SLAVE_COMMAND, PIC_EOI);
    }
    outb(P_PIC_MASTER_COMMAND, PIC_EOI);
}

u16 start_all_irq() {
    u8 oldst_m = inb(P_PIC_MASTER);
    u8 oldst_s = inb(P_PIC_SLAVE);

    start_irq(SLAVE_IRQ);
    start_irq(KEYBOARD_IRQ);
    start_irq(PIT_TIMER_IRQ);
    start_irq(RTC_TIMER_IRQ);


    return ((oldst_m << 8)) + oldst_s;
}