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

void disk_reset()
{
	// outb(0x3F6,0);
	// outb(0x3F6,4);
	// outb(0x3F6,0);
	// while (true)
	// {
	// 	byte rs = inb(P_STATUS);
	// 	//如果没准备好
	// 	if( (bit(rs,0)==1) || (bit(rs,5)==1) || (bit(rs,7)==1) ||
	// 		(bit(rs,6)==0) || (bit(rs,3)==0) ) 
	// 		{
	// 			break;
	// 		}
	// }
}

void wait_busy()
{
	while((bit(inb(P_STATUS),7)==1))
	{
		printf("waiting disk busy...");
		//等待读取结束
	}
	printf("busy wait over.\n");
}

void wait_data()
{
	wait_busy();
	while(bit(inb(P_STATUS),3)==0)
	{
		printf("waiting disk data...");
		//等待读取结束
	}
	printf("data wait over.\n");
}

bool disk_read(void* pbuf, u16 len, u6b lba)
{
	char* buf = (char*)pbuf;
	//disk_reset();
	//printf("disk reset\n");

	int n512b = (len>>9)+1;//ceil(len/512f)

	wait_busy();
	outb(P_DRIVE,0x64);//master disk
	outb(P_ERROR,0);
	outb(P_ERROR,0);

	//传递参数
	// inb(P_DATA);
	// inb(P_DATA);
	// inb(P_DATA);
	// inb(P_DATA);//4 times delay for ready
	outb(P_COUNT_SECTOR,n512b>>8);
	outb(P_COUNT_SECTOR,n512b);
	outb(P_LBA_HIGH,lba.bytes[5]);
	outb(P_LBA_MID,lba.bytes[4]);
	outb(P_LBA_LOW,lba.bytes[3]);
	outb(P_LBA_HIGH,lba.bytes[2]);
	outb(P_LBA_MID,lba.bytes[1]);
	outb(P_LBA_LOW,lba.bytes[0]);

	//传递命令
	outb(P_INSTRUCTION,0x24);//读取

	wait_data();
	//byte rs = inb(P_STATUS);
	
	// //如果出错
	// if( (bit(rs,0)==1) || (bit(rs,5)==1) || 
	// 	(bit(rs,6)==0) || (bit(rs,3)==0) )
	// {
	// 	return false;
	// }

	asm("pushf");
    asm("cld");
	// asm("mov dx,%0"::"g"(P_DATA));
	// for(int i=0;i<n512b-1;i++)
	// {
    // 	asm("rep insw"::"c"(256),"D"(buf));//读取整的扇区
	// 	buf+=512;
	// }
	// char b[512];
	// asm("rep insw"::"c"(256),"D"(b));

	for(int i=0;i<len/2;i++)
	{
		word w = inw(P_DATA);
		*buf=w;
		buf++;
		*buf=w>>8;
		buf++;
	}
	if(len&0x1)
	{
		word w = inw(P_DATA);
		*buf=w;
	}

    asm("popf");

	asm("":::"cx","di","dx");
	// memcpy(buf,b,len%512);
	return true;
}

bool disk_write(void* pbuf, u16 len, u6b lba)
{
	char* buf = (char*)pbuf;
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
	
	// //如果出错
	// if( (bit(rs,0)==1) || (bit(rs,5)==1) || 
	// 	(bit(rs,6)==0) || (bit(rs,3)==0) )
	// {
	// 	return false;
	// }

	for(u16 i=0;i<len;i++)
	{
		outw(P_DATA, *(word*)buf);
		buf++;
		nop();//delay
	}

	outw(P_INSTRUCTION,0x34);//刷新
	wait_busy();
	// //如果出错
	// if( (bit(rs,0)==1) || (bit(rs,5)==1) || 
	// 	(bit(rs,6)==0) || (bit(rs,3)==0) )
	// {
	// 	return false;
	// }

	return true;
}
