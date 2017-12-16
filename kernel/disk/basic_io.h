#ifndef BASIC_IO_H
#define BASIC_IO_H

#include "types.h"
#include "../asm.h"

/**
 * 初始化硬盘
 */
void disk_init();

/** 
 * 硬盘读写
 * @note   务必注意@param len
 * @param  pbuf:数据缓冲区 
 * @param  nSec:读取扇区数
 * @param  lba:逻辑块号
 * @retval 是否成功
 */
bool disk_read(void* pbuf, u16 nSec, u6b lba);
bool disk_write(void* pbuf, u16 nSec, u6b lba);

#endif //BASIC_IO_H
