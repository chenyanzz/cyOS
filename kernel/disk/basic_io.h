#ifndef BASIC_IO_H
#define BASIC_IO_H

#include "types.h"
#include "../asm.h"
/** 
 * 
 * @brief  cy
 * @note   硬盘读写
 * @param  buf:数据缓冲区 
 * @param  len:读写长度
 * @param  lba:逻辑块号
 * @retval 是否成功
 */
bool disk_read(void* buf, u16 len, u6b lba);
bool disk_write(void* buf, u16 len, u6b lba);

#endif //BASIC_IO_H
