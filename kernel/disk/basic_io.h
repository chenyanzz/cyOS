#ifndef BASIC_IO_H
#define BASIC_IO_H

#include "types.h"
#include "../asm.h"
/** 
 * 硬盘读写
 * @note   务必注意@param len
 * @param  pbuf:数据缓冲区 
 * @param  len:读写长度
 * 			一定要整除2（因为读写都是word为单位）
 * 			最好是扇区大小整倍数
 * @param  lba:逻辑块号
 * @retval 是否成功
 */
bool disk_read(void* pbuf, u16 len, u6b lba);
bool disk_write(void* pbuf, u16 len, u6b lba);

#endif //BASIC_IO_H
