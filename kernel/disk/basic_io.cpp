#include "basic_io.h"
#include "asm.h"
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
const word P_COMMAND 		= 0x1F7;
const word P_STATUS 		= 0x1F7;

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

bool init_disk()
{
	outb(P_DRIVE,0x64);//master disk
	outb(P_ERROR,0);
	outb(P_ERROR,0);
	wait_busy();
	return (!iserr());
}

bool disk_read(void* pbuf, u16 nSec, lba48 lba)
{
	word* buf = (word*)pbuf;

	wait_busy();
	// outb(P_DRIVE,0x64);//master disk
	// outb(P_ERROR,0);
	// outb(P_ERROR,0);
	//传递参数
	outb(P_COUNT_SECTOR,nSec>>8);
	outb(P_LBA_LOW,lba>>(3*8));
	outb(P_LBA_MID,lba>>(4*8));
	outb(P_LBA_HIGH,lba>>(5*8));
	outb(P_COUNT_SECTOR,nSec>>0);
	outb(P_LBA_LOW,lba>>(0*8));
	outb(P_LBA_MID,lba>>(1*8));
	outb(P_LBA_HIGH,lba>>(2*8));

	//传递命令
	outb(P_COMMAND,0x24);//读取

	wait_data();

	if(iserr())
	{
		return false;
	}
	
	//读取数据
	for(u64 i=0;i<nSec*256;i++)
	{
		buf[i] = inw(P_DATA);
		//printf("%lld\n",i);
	}

	return true;
}

bool disk_write(void* pbuf, u16 nSec, lba48 lba)
{
	word* buf = (word*)pbuf;

	wait_busy();
	// outb(P_DRIVE,0x64);//master disk
	// outb(P_ERROR,0);
	// outb(P_ERROR,0);

	//传递参数
	outb(P_COUNT_SECTOR,nSec>>8);
	outb(P_LBA_LOW,lba>>(3*8));
	outb(P_LBA_MID,lba>>(4*8));
	outb(P_LBA_HIGH,lba>>(5*8));
	outb(P_COUNT_SECTOR,nSec>>0);
	outb(P_LBA_LOW,lba>>(0*8));
	outb(P_LBA_MID,lba>>(1*8));
	outb(P_LBA_HIGH,lba>>(2*8));

	//传递命令
	outb(P_COMMAND,0x34);//写入

	wait_data();

	if(iserr())
	{
		return false;
	}

	//写入数据
	for(int i=0;i<nSec;i++)
	{
		for(int j=0;j<256;j++)
		{
			outw(P_DATA,buf[j]);
		}
		buf+=256;
	}
	
	outb(P_COMMAND,0xE7);
	wait_busy();
	if(iserr())
	{
		return false;
	}

	return true;
}