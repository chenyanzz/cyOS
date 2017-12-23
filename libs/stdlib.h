#ifndef STDLIB_H
#define STDLIB_H

void memset(void*addr,char val,int size);
void memcpy(void*to,void*from,int size);

/**
* 交换两个字符位置
* @param a 待交换字符地址
* @param b 待交换字符地址
*/
void swap(char* a, char* b);

/**
* 颠倒字符串
* @param str 字符串
* @param len 字符串长度
*/
void strReverse(char* str, int len);

/**
* 字符串填充字符
* @param str 字符串
* @param c 填充字符
* @param len 字符串长度
*/
void strFill(char* str, char c, int len);

/**
* 十进制数字所占位数
* @param num 数字
* @return 位数
*/
void numLen(long long num);

/**
* 从字符串开头解析整数
* @param pc 字符串
* @return 整数（找不到为0）
*/
int parseNum(const char* pc);

/**
 * 获取字符串长度，0为结尾
 */
int strlen(char* str);

#endif //STDLIB_H
