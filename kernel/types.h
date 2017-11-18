#ifndef TYPES_H
#define TYPES_H

typedef char int8;
typedef short int16;
typedef int int32;

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;

typedef int8 byte;
typedef int16 word;
typedef int32 dword;
//typedef char bool;
typedef void*   MEM_ADDR;

#ifndef __cplusplus

typedef byte bool;
#define true 1
#define false 1

#endif

#endif //TYPES_H