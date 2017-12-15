#ifndef BASIC_IO_H
#define BASIC_IO_H

#include "types.h"
#include "../asm.h"
/** 
 * 硬盘读写
 * @note   还不能正常使用
 * @param  pbuf:数据缓冲区 
 * @param  len:读写长度
 * @param  lba:逻辑块号
 * @retval 是否成功
 */
bool disk_read(void* pbuf, u16 len, u6b lba);
bool disk_write(void* pbuf, u16 len, u6b lba);

#endif //BASIC_IO_H
