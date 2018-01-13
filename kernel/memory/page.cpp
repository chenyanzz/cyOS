#include "page.h"
#include "asm.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"

MEM_LIST_ITEM *mem_list = (MEM_LIST_ITEM *)p_mem_list_items;
u16 count_mem_list;
u32 memory_size;

//1 bit 	-> 4k
//1 byte	-> 32K
//32 bytes 	-> 1M
//1M		-> 32G
byte pageUsage[MAX_MEMORY_MB * 32];



bool init_mem_page()
{
	count_mem_list = *p_count_mem_list;

	//将操作系统使用的部分及boot使用部分设置为used
	mem_list[count_mem_list++]={0x0,1024*1024*10,MEM_LIST_ITEM::SystemUsed,0};//kernel
	mem_list[count_mem_list++]={0x90000,0x1000,MEM_LIST_ITEM::SystemUsed,0};//boot.data

	//print_mem_list();

	//先全部初始化为使用中
	memset(pageUsage, 0xFF, sizeof(pageUsage));

	//将未使用内存块设置为0
	for (int i = 0; i < count_mem_list; i++)
	{
		MEM_LIST_ITEM &item = mem_list[i];
		u32 startb = ceil((double)item.base_addr /SIZE_MEM_PAGE);
		u32 lenb = floor((double)item.length /SIZE_MEM_PAGE);
		memory_size = max(memory_size,startb+lenb);
		if (item.type == MEM_LIST_ITEM::Unused)
		{
			for (u32 i = 0; i < lenb; i++)
			{
				u32 pos = startb + i;
				clrb(pageUsage[pos / 8], pos % 8);
			}
		}
		else
		{
			for (u32 i = 0; i < lenb; i++)
			{
				u32 pos = startb + i;
				setb(pageUsage[pos / 8], pos % 8);
			}
		}
	}
}

#ifdef OS_DEBUG

void print_mem_list()
{
	for (int i = 0; i < count_mem_list; i++)
	{
		MEM_LIST_ITEM &item = mem_list[i];
		printf("from %llx\t",item.base_addr);
		printf("to %llx\t",item.base_addr+item.length);
		printf("size=%llx\t",item.length);
		printf("type=%s\n",item.type==MEM_LIST_ITEM::Unused?"unused":"using");
	}
}

#endif // OS_DEBUG

void* getFreePage()
{
	u32 i=0;
	while(i<sizeof(pageUsage))
	{
		if(bit(pageUsage[i / 8], i % 8)==0)
		{
			setb(pageUsage[i / 8], i % 8);
			break;
		}
		i++;
	}
	if(i==sizeof(pageUsage))
	{
		return -1;
	}

	return (void*)(i*SIZE_MEM_PAGE);
}