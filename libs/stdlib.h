#pragma once
#define STDLIB_H

#include "string.h"

/*参见标准C函数的使用方法*/

void memset(void*addr,char val,int size);
void memcpy(void*to,void*from,int size);

/**
 * 十进制数字所占位数
 * @param num 数字
 * @return 位数
 */
void numLen(long long num);


