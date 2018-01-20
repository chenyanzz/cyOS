#pragma once

#include "types.h"
#include "asm.h"

/**
 * 初始化硬盘
 */
bool init_disk();

///逻辑扇区号类型
typedef u64 lba48;

/** 
 * 硬盘读
 * @param  pbuf:数据缓冲区 
 * @param  nSec:读取扇区数
 * @param  lba:逻辑块号
 * @retval 是否成功
 */
bool disk_read(void* pbuf, u16 nSec, lba48 i64lba);

/**
 * 硬盘写
 * @param  pbuf:数据缓冲区
 * @param  nSec:读取扇区数
 * @param  lba:逻辑块号
 * @retval 是否成功
 */
bool disk_write(void* pbuf, u16 nSec, lba48 i64lba);


