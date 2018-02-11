#pragma once

#include "types.h"

#define IDT_NUM 256

typedef void (*INTERRUPT_CALLBACK)();

struct Idt_Descriptor //in idtr
{
	//word SPACER;
	word size;
	void* addr;
};

enum GateType
{
	INT = 0b01110,
	FAULT = 0b01110, ///<重新执行触发的命令
	TRAP = 0b01111,  ///<执行触发的命令的下一条
};

struct Interrupt_Decriptor
{
	u16 offset_0_15;	  ///< offset bits 0..15 地址低16位
	u16 segment_selector; ///< a code segment selector in GDT or LDT 段号
	u8 zero = 0;		  ///< unused, set to 0 必须为0
	GateType type : 5;	  ///< 此中断类型
	RPL rpl : 2;	      ///< 权限级别
	bool present : 1;     ///< 是否存在
	u16 offset_16_31;     ///< offset bits 16..31 地址高16位
};

/**
 * 初始化IDt
 */
bool init_IDT();

/**
 * 同汇编lidt，加载idtr
 */
void lidt(Interrupt_Decriptor idtr);

/**
 * 设置IDT表项
 * @param index 第几个中断
 * @param item 那个中断描述符
 */
void set_idt_item(u8 index, Interrupt_Decriptor item);

/**
 * 设置一个门
 * @param index 第几个中断
 * @param func 回调的函数
 * @param gatetype 门的类型
 */
void set_gate(u8 index, INTERRUPT_CALLBACK func, GateType gatetype);


