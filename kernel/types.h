#ifndef TYPES_H
#define TYPES_H

//定义32位下的类型别名 


typedef char 	int8;
typedef short 	int16;
typedef int 	int32;
typedef char 	i8;
typedef short 	i16;
typedef int 	i32;
typedef char 	i1b;
typedef short 	i2b;
typedef int 	i4b;

typedef unsigned char 	uint8;
typedef unsigned short 	uint16;
typedef unsigned int 	uint32;
typedef unsigned char 	u8;
typedef unsigned short 	u16;
typedef unsigned int 	u32;
typedef unsigned char 	u1b;
typedef unsigned short 	u2b;
typedef unsigned int 	u4b;

typedef int8 	byte;
typedef int16 	word;
typedef int32 	dword;

typedef union u6b
{
	byte bytes[6];
	word words[3];
} u48;

typedef union u8b
{
	byte bytes[8];
	word words[4];
} u64;


typedef void*	MEM_ADDR;

//给C语言定义bool类型
#ifndef __cplusplus

typedef byte 	bool;
const bool true  = 1;
const bool false = 0;

#endif

#endif //TYPES_H