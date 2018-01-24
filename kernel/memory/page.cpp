#include "page.h"
#include "asm.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"

MEM_LIST_ITEM *mem_list = p_mem_list_items;
u16 count_mem_list;
u64 memory_total_size;

//1 bit 	-> 4k
//1 byte	-> 32K
//32 bytes 	-> 1M
//1M		-> 32G
const int MAX_MEM_PAGES = MAX_MEMORY_MB * 32;
byte pageUsage[MAX_MEM_PAGES];


bool init_mem_page()
{
//	printf("\n%ld",(&AAA)-pageUsage);

	count_mem_list = *p_count_mem_list;

	//将操作系统使用的部分及boot使用部分设置为used
	mem_list[count_mem_list++]={0x0,KERNEL_TOTAL_SIZE,MEM_LIST_ITEM::SystemUsed,0};//kernel
	mem_list[count_mem_list++]={0x90000,0x1000,MEM_LIST_ITEM::SystemUsed,0};//boot.data

    memory_total_size=0;

	//先全部初始化为使用中
	memset(pageUsage, 0xFF, sizeof(pageUsage));

	//将未使用内存块设置为0
	for (int i = 0; i < count_mem_list; i++)
	{
		MEM_LIST_ITEM &item = mem_list[i];
		u32 startp = ceil((double)item.base_addr /SIZE_MEM_PAGE);//<开始页
		u32 lenp = floor((double)item.length /SIZE_MEM_PAGE);//<几个页

        if(startp>MAX_MEM_PAGES)
        {
            continue;
        }

        //如果 超过最大页数就限制到最大页数
        if ((startp+lenp)>MAX_MEM_PAGES)
        {
            lenp = MAX_MEM_PAGES-startp -1;
        }

        //检测是否是已知最大内存
		memory_total_size = max(memory_total_size,(item.base_addr+item.length));

		if (item.type == MEM_LIST_ITEM::Unused)
		{
			for (u32 i = 0; i < lenp; i++)
			{
				u32 pos = startp + i;
				clrb(pageUsage[pos / 8], pos % 8);
			}
		}
		else
		{
			for (u32 i = 0; i < lenp; i++)
			{
				u32 pos = startp + i;
				setb(pageUsage[pos / 8], pos % 8);
			}
		}
	}

//    print_mem_list();

	return true;
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
	printf("memory size total: =%xMB",memory_total_size/(1024*1024));
}

#endif

void* allocPage()
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
		return (void*)-1;
	}

	return (void*)(i*SIZE_MEM_PAGE);
}

void freePage(void* page)
{
	u32 bit = (u32)page/4096;
	clrb(pageUsage[bit/8],bit%8);
}