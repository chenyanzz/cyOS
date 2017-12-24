#ifndef STDLIB_H
#define STDLIB_H

#include "string.h"


void memset(void*addr,char val,int size);
void memcpy(void*to,void*from,int size);

/**
 * 十进制数字所占位数
 * @param num 数字
 * @return 位数
 */
void numLen(long long num);

#endif //STDLIB_H
