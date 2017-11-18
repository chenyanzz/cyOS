#ifndef DEFINE_H
#define DEFINE_H

/*
本文件是一些系统常用宏定义
*/

#include "asm.h"
#include "types.h"
//底层相关部分

#define p_cursor_x  ((char*)0x90000)
#define p_cursor_y  ((char*)0x90001)
#define p_extend_memory_size  ((short*)0x90003)
#define p_firstChar  ((char*)0xB8000)



//将段号（0~2047）变成段选择符号
#define gknlseg(n) (SEGMENT)(n<<3+0b000)
#define gusrseg(n) (SEGMENT)(n<<3+0x011)

#define lknlseg(n) (SEGMENT)(n<<3+0b100)
#define lusrseg(n) (SEGMENT)(n<<3+0b111)

#define KNL_PROGRAM_SEG gknlseg(1)
#define KNL_DATA_SEG gknlseg(2)



#endif