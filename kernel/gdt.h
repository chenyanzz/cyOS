#pragma once

#include "types.h"
#include "asm.h"

//GDT的项数
#define GDT_NUM 256

///段的类型
enum SEGMENT_TYPE {
    DATA_R = 0b0000,
    DATA_RW = 0b0010,
    CODE_R = 0b1000,
    CODE_RW = 0b1010
};


///段表 表项
///@note for more info,see Intel Manual P.2756
struct segment_descriptor {
    //0
    unsigned seglmt_0_15 :16;
    //16
    unsigned baseaddr_0_23 :24;
    //40

    enum SEGMENT_TYPE segtype :4;
    enum S//选择符类型
    {
        SYSTEM = 0,///<莫名其妙，一般选第二个
        CODE_DATA = 1
    } s :1;
    RPL rpl :2;
    bool present :1;///<是否存在
    //48
    unsigned seglmt_16_19 :4;
    unsigned AVL :1;//available for use by system software？？！！
    unsigned zero :1;
    enum D {
        BIT_32=1,
        BIT_16=0
    } d :1;
    enum G {    //limit的单位
        UNIT_1BYTE = 0,//bytes=limit*1
        UNIT_4K = 1//bytes=limit*4k
    } g :1;
    unsigned baseaddr_24_31 :8;

};


/**
 * 构建一个段描述符
 */
segment_descriptor make_descriptor(int limit, int baseaddr, SEGMENT_TYPE segment_type, RPL rpl);

struct gdt_descriptor//in gdtr GDTR寄存器里的东西
{
    word size;
    void *addr;
};

///GDT表项
typedef segment_descriptor global_descriptor;

///LDT表项
typedef segment_descriptor local_descriptor;

/**
 * 设置一个GDT描述符
 * @param index 第几个
 * @param item 描述符
 */
void set_gdt_item(u8 index, global_descriptor item);

/**
 * 初始化GDT表
 * GDT[0]内核代码段
 * GDT[1]内核数据段
 */
bool init_GDT();
