#include "basic_io.h"
#include "asm.h"
#include "../asm.h"
const word P_DATA 			= 0x1F0;
const word P_ERRCODE 		= 0x1F1;
const word P_COUNT_SECTOR 	= 0x1F2;
const word P_LBA_LOW 		= 0x1F3;
const word P_LBA_MID 		= 0x1F4;
const word P_LBA_HIGH 		= 0x1F5;
const word P_DRIVE			= 0x1F6;
const word P_INSTRUCTION 	= 0x1F7;
const word P_STATUS = P_INSTRUCTION;

void disk_reset()
{
	outb(0x3F6,0);
	outb(0x3F6,4);
	outb(0x3F6,0);
	while (true)
	{
		byte rs = inb(P_STATUS);
		//如果没准备好
		if( (bit(rs,0)==1) || (bit(rs,5)==1) || (bit(rs,7)==1) ||
			(bit(rs,6)==0) || (bit(rs,3)==0) ) 
			{
				break;
			}
	}
}

void wait_busy()
{
	while(bit(inb(P_INSTRUCTION),7)==1);//等待读取结束
}

bool disk_read(void* buf, u16 len, u6b lba)
{
	int n512b = (len+511)>>9;//ceil(len/512f)

	//传递参数
	outb(P_DRIVE,0x50);//master disk
	outb(P_COUNT_SECTOR,n512b>>8);
	outb(P_COUNT_SECTOR,n512b>>8);
	outb(P_COUNT_SECTOR,n512b>>8);
	outb(P_COUNT_SECTOR,n512b>>8);//4 times delay for ready
	outb(P_COUNT_SECTOR,n512b>>8);
	outb(P_LBA_HIGH,lba.bytes[5]);
	outb(P_LBA_MID,lba.bytes[4]);
	outb(P_LBA_LOW,lba.bytes[3]);
	outb(P_COUNT_SECTOR,n512b);
	outb(P_LBA_HIGH,lba.bytes[2]);
	outb(P_LBA_MID,lba.bytes[1]);
	outb(P_LBA_LOW,lba.bytes[0]);

	//传递命令
	outb(P_INSTRUCTION,0x24);//读取

	wait_busy();
	
	byte rs = inb(P_STATUS);
	
	//如果出错
	if( (bit(rs,0)==1) || (bit(rs,5)==1) || 
		(bit(rs,6)==0) || (bit(rs,3)==0) )
	{
		return false;
	}

	asm("pushf");
    asm("cld");
	for(int i=0;i<n512b;i++)
	{
    	asm("rep insw"::"c"(256),"D"(buf));
		buf+=512;
	}
    asm("popf");

	return true;
}

bool disk_write(void* buf, u16 len, u6b lba)
{

	int n512b = (len+511)>>9;//ceil(len/512f)

	//传递参数
	outb(P_DRIVE,0x50);//master disk
	outb(P_COUNT_SECTOR,n512b>>8);
	outb(P_COUNT_SECTOR,n512b>>8);
	outb(P_COUNT_SECTOR,n512b>>8);
	outb(P_COUNT_SECTOR,n512b>>8);//4 times delay for ready
	outb(P_COUNT_SECTOR,n512b>>8);
	outb(P_LBA_HIGH,lba.bytes[5]);
	outb(P_LBA_MID,lba.bytes[4]);
	outb(P_LBA_LOW,lba.bytes[3]);
	outb(P_COUNT_SECTOR,n512b);
	outb(P_LBA_HIGH,lba.bytes[2]);
	outb(P_LBA_MID,lba.bytes[1]);
	outb(P_LBA_LOW,lba.bytes[0]);

	//传递命令
	outb(P_INSTRUCTION,0x34);//写入

	wait_busy();
	
	byte rs = inb(P_STATUS);
	
	//如果出错
	if( (bit(rs,0)==1) || (bit(rs,5)==1) || 
		(bit(rs,6)==0) || (bit(rs,3)==0) )
	{
		return false;
	}

	for(u16 i=0;i<len;i++)
	{
		outw(P_DATA, *(word*)buf);
		buf++;
		nop();//delay
	}

	outw(P_INSTRUCTION,0x34);//刷新

	//如果出错
	if( (bit(rs,0)==1) || (bit(rs,5)==1) || 
		(bit(rs,6)==0) || (bit(rs,3)==0) )
	{
		return false;
	}

	return true;
}
