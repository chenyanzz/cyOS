#include "basic_io.h"
#include "asm.h"
#include "../asm.h"
#include "stdlib.h"
#include "stdio.h"
const word P_DATA 			= 0x1F0;
const word P_ERROR			= 0x1F1;
const word P_COUNT_SECTOR 	= 0x1F2;

//LBA方式
const word P_LBA_LOW 		= 0x1F3;
const word P_LBA_MID 		= 0x1F4;
const word P_LBA_HIGH 		= 0x1F5;

const word P_DRIVE			= 0x1F6;
const word P_INSTRUCTION 	= 0x1F7;
const word P_STATUS = P_INSTRUCTION;

void wait_busy()
{
	while((bit(inb(P_STATUS),7)==1))
	{
		// printf("waiting disk busy...");
		//等待读取结束
	}
	// printf("busy wait over.\n");
}

void wait_data()
{
	wait_busy();
	while(bit(inb(P_STATUS),3)==0)
	{
		// printf("waiting disk data...");
		//等待读取结束
	}
	// printf("data wait over.\n");
}

bool iserr()
{
	byte rs = inb(P_STATUS);

	if((bit(rs,0)==1)	//错误
	|| (bit(rs,5)==1) 	//故障
	|| (bit(rs,6)==0))	//停转
	{
		return true;
	}

	return false;
}

bool disk_read(void* pbuf, u16 len, u6b lba)
{
	word* buf = (word*)pbuf;
	int n512b = (len>>9)+1;//ceil(len/512f)

	wait_busy();
	outb(P_DRIVE,0x64);//master disk
	outb(P_ERROR,0);
	outb(P_ERROR,0);

	//传递参数
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

	wait_data();

	if(iserr())
	{
		return false;
	}

	//读取数据
	for(int i=0;i<len/2;i++)
	{
		word w = inw(P_DATA);
		buf[i]=w;
	}

	asm("":::"cx","di","dx");

	return true;
}

bool disk_write(void* pbuf, u16 len, u6b lba)
{
	word* buf = (word*)pbuf;
	int n512b = (len>>9)+1;//ceil(len/512f)

	wait_busy();
	outb(P_DRIVE,0x64);//master disk
	outb(P_ERROR,0);
	outb(P_ERROR,0);

	//传递参数
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

	if(iserr())
	{
		return false;
	}

	//写入数据
	for(int i=0;i<len/2;i++)
	{
		outw(P_DATA,buf[i]);
	}

	if(iserr())
	{
		return false;
	}

	wait_busy();

	asm("":::"cx","di","dx");

	return true;
}
