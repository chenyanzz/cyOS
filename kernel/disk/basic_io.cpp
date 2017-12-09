#include "basic_io.h"
/*有关CHS方式磁盘读写的端口说明

端口号     读还是写   具体含义
1F0H       读/写      用来传送读/写的数据(其内容是正在传输的一个字节的数据)
1F1H       读         用来读取错误码
1F2H       读/写      用来放入要读写的扇区数量
1F3H       读/写      用来放入要读写的扇区号码
1F4H       读/写      用来存放读写柱面的低8位字节
1F5H       读/写      用来存放读写柱面的高2位字节(其高6位恒为0)
1F6H       读/写      用来存放要读/写的磁盘号及磁头号
	第7位     恒为1
	第6位     CHS方式=0，LBA方式=1
	第5位     恒为1
	第4位     为0代表第一块硬盘、为1代表第二块硬盘
	第3~0位    用来存放要读/写的磁头号
1f7H
	--读：用来存放读操作后的状态
		第7位     控制器忙碌
		第6位     磁盘驱动器准备好了
		第5位     写入错误
		第4位     搜索完成
		第3位     为1时扇区缓冲区没有准备好
		第2位     是否正确读取磁盘数据
		第1位     磁盘每转一周将此位设为1,
		第0位     之前的命令因发生错误而结束
	--写：该位端口为命令端口,用来发出指定命令
		为50h     格式化磁道
		为20h     尝试读取扇区
		为21h     无须验证扇区是否准备好而直接读扇区
		为22h     尝试读取长扇区(用于早期的硬盘,每扇可能不是512字节,而是128字节到1024之间的值)
		为23h     无须验证扇区是否准备好而直接读长扇区
		为30h     尝试写扇区
		为31h     无须验证扇区是否准备好而直接写扇区
		为32h     尝试写长扇区
		为33h     无须验证扇区是否准备好而直接写长扇区
*/

const u8 P_CYLINDER 	= 0x1F4;
const u8 P_HEADER 		= 0x1F6;
const u8 P_START_SECTOR = 0x1F4;
const u8 P_COUNT_SECTOR = 0x1F2;
const u8 P_INSTRUCTION 	= 0x1F7;
const u8 P_RESULT		= 0x1F7;

extern int floor(double);

byte readchs(void* buf, int len, u16 c, u8 h, u16 s)
{
	//传递磁盘寻址参数
	//H: 磁头
	outb(P_HEADER,0b10100000+h);

	//C：柱面
	outw(P_CYLINDER,c);//两个8位端口可以当做一个16位端口

	//S: 扇区
	outb(P_START_SECTOR,s);

	//传递命令
	outb(P_INSTRUCTION,0x20);//尝试读取
	while(bit(inb(P_INSTRUCTION),7));//等待读取结束
	if(bit(inb(P_INSTRUCTION),3));//如果没有数据
		return false;
}

void writechs(void* buf, int len, u16 c, u8 h, u16 s)
{
	
}