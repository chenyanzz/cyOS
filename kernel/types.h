#pragma once

/**
 * 定义32位下的类型别名
 */

typedef char int8;
typedef short int16;
typedef int int32;
typedef long long int64;
typedef int8 i8;
typedef int16 i16;
typedef int32 i32;
typedef int64 i64;
typedef i8 i1b;
typedef i16 i2b;
typedef i32 i4b;
typedef i64 i8b;

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;

typedef uint8 u8;
typedef uint16 u16;
typedef uint32 u32;
typedef uint64 u64;

typedef u8 u1b;
typedef u16 u2b;
typedef u32 u4b;
typedef u64 uint64;

typedef uint8 byte;
typedef uint16 word;
typedef uint32 dword;
typedef uint64 qword;

/*权限级别*/
enum RPL {
    KERNEL = 0,///<ring 0
    USER = 3///<ring 3
};

typedef uint32 id_t;
typedef uint32 id_count_t;
typedef uint32 size_t;