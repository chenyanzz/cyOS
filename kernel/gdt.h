#ifndef GDT_H
#define GDT_H

#include "types.h"
#include "asm.h"

//number of global descriptors
#define GDT_NUM 256

enum SEGMENT_TYPE
{
	DATA_R=0b0000,
	DATA_RW=0b0010,
	CODE_R=0b1000,
	CODE_RW=0b1010
};

//for more info,see Intel Manual P.2756
struct segment_descriptor
{
    //0
    unsigned seglmt_0_15		:16;
    //16
	unsigned baseaddr_0_15		:16;
    //32
	unsigned baseaddr_16_23		:8;    
	enum SEGMENT_TYPE segtype	:4;
    enum S//选择符类型
    {
        SYSTEM=0,//莫名其妙，一般选第二个
        CODE_DATA=1
    }s          				:1;
    RPL rpl	                    :2;
	bool present			    :1;//是否存在
    //48
	unsigned seglmt_16_19		:4;
	unsigned AVL				:1;//available for use by system software
	bool is_bit_64					:1;
	bool is_bit_16				    :1;
    enum UNIT_SIZE
    {
        SIZE_1BYTE=0,//bytes=limit*1
        SIZE_4K=1//bytes=limit*4k
    }unit_size   				:1;
    unsigned baseaddr_24_31		:8;
    
};

#define make_descriptor(limit,baseaddr,SEGMENT_TYPE,RPL,UNIT_SIZE)    \
{   \
    limit,baseaddr,baseaddr>>16,SEGMENT_TYPE,segment_descriptor::CODE_DATA,segment_descriptor::RPL,true,limit>>24,  \
    false,false,false,segment_descriptor::UNIT_SIZE,baseaddr>>24  \
}

struct gdt_descriptor//in gdtr
{
    word size;
    MEM_ADDR addr;
};

typedef segment_descriptor global_descriptor;
typedef segment_descriptor local_descriptor;

// void set_GDT(int index,global_descriptor gd);
bool init_GDT();

#endif //GDT_H