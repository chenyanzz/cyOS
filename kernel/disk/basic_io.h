#ifndef BASIC_IO_H
#define BASIC_IO_H

#include "types.h"

/** 
 * 
 * @brief  cy
 * @note   硬盘读写
 * @param  buf:数据缓冲区 
 * @param  len:读写长度
 * @param  c:10位 cylinder 柱面
 * @param  h:4位 header 磁头
 * @param  s:8位 sector 扇区
 * @retval 是否成功
 */
bool disk_read(void* buf, int len, u16 c, u8 h, u16 s);
bool disk_write(void* buf, int len, u16 c, u8 h, u16 s);

#endif //BASIC_IO_H
