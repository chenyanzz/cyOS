/**
 * 内存分页相关
 */

#pragma once

#include "types.h"

#define SIZE_MEM_PAGE (4096)

//注意：这个是包含了程序、变量、栈的总大小
#define KERNEL_TOTAL_SIZE (1024*1024*20)//与栈底保持相等

struct MEM_LIST_ITEM
{
	/*
	First uint64_t = Base address
	Second uint64_t = Length of "region" (if this value is 0, ignore the entry)
	Next uint32_t = Region "type"
	*/
	u64 base_addr;
	u64 length;
	enum MemType
	{
		/*
		Value	Pneumonic		Description
		1	AddressRangeMemory	This run is available RAM usable by the operating system.
		2	AddressRangeReserved	This run of addresses is in use or reserved by the system, and must not be used by the operating system.
		Other	Undefined		Undefined - Reserved for future use.  Any range of this type must be treated by the OS as if the type returned was AddressRangeReserved.
		*/
		
		/*
		Type 1: Usable (normal) RAM
		Type 2: Reserved - unusable
		Type 3: ACPI reclaimable memory
		Type 4: ACPI NVS memory
		Type 5: Area containing bad memory
		 */
		Unused = 1,
		Reserved = 2,
		ACPI_RECLAIMABBLE = 3,
		ACPI_NVS = 4,
		Bad = 5,

		SystemUsed = 250
	} type;
	u32 attrbutes; //unused
};

extern MEM_LIST_ITEM *mem_list;
extern u16 count_mem_list;
extern u64 memory_total_size;
#define MAX_MEMORY (0xFFFFFFFF) ///<最大内存4G

//1代表使用中，0代表未使用
extern byte pageUsage[];

/**
 * 初始化内存页表 
 */
bool init_mem_page();

#ifdef OS_DEBUG
/**
 * 打印从BIOS获取的mem_list
 */
void print_mem_list();


/**
 * 获取没用过的一页
 * @return 那一页的首地址
 * @retval -1 无空页？？！！
 */
void* allocPage();

/**
 * 释放一内存页 
 */
void freePage(void* page);

#endif
