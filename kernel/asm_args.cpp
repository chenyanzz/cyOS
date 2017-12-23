// #include "asm_args.h"

// struct MEM_LIST_ITEM
// {
// 	/*
// 	First uint64_t = Base address
// 	Second uint64_t = Length of "region" (if this value is 0, ignore the entry)
// 	Next uint32_t = Region "type"
// 	*/
// 	u64 base_addr;
// 	u64 length;
// 	enum MemType
// 	{
// 		/*
// 		Type 1: Usable (normal) RAM
// 		Type 2: Reserved - unusable
// 		Type 3: ACPI reclaimable memory
// 		Type 4: ACPI NVS memory
// 		Type 5: Area containing bad memory
// 		 */
// 		Unused = 1,
// 		Reserved = 2,
// 		ACPI_RECLAIMABBLE = 3,
// 		ACPI_NVS = 4,
// 		Bad = 5
// 	}type;
// 	u32 attrbutes;//unused

// };

// const u8* p_cursor_x 			= ((u8*)0x90000);
// const u8* p_cursor_y 			= ((u8*)0x90001);
// const u16* p_extend_memory_size = ((u16*)0x90003);
// const u16* p_count_mem_list 	= ((u16*)0x90030);
// const MEM_LIST_ITEM* p_mem_list_items 	= ((MEM_LIST_ITEM*)0x90040);

// const u8* p_firstChar 			= ((u8*)0xB8000);