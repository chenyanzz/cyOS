#ifndef IDT_H
#define IDT_H

#include "types.h"

#define IDT_NUM 256

#define NAKED __attribute__((fastcall))

typedef void (*INTERRUPT_CALLBACK) ();

struct idt_descriptor//in idtr
{
	//word SPACER;
    word size;
    MEM_ADDR addr;
};

enum GateType
{
	INT_GATE	= 0b01110,
	TRAP_GATE	= 0b01111
};

struct interrupt_decriptor
{
	u16 offset_0_15;	  // offset bits 0..15
	u16 segment_selector; // a code segment selector in GDT or LDT
	u8 zero = 0;		  // unused, set to 0
	GateType type:5;
	RPL rpl:2;
	bool present:1;
	u16 offset_16_31;	  // offset bits 16..31
};

bool init_IDT();

void lidt(interrupt_decriptor lidt);

void set_idt_item(u8 index,interrupt_decriptor item);

void set_gate(u8 index, INTERRUPT_CALLBACK func, GateType gatetype);

#endif //IDT_H
