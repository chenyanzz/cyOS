/*实现一些经典的常用的字符串操作*/

#pragma once
#define STRING_H

#include "types.h"

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
 * 获取字符串长度，0为结尾
 * @param str 字符串指针
 * @retval 长度，不包括结尾的0
 */
size_t strlen(char* str);

/**
 * 字符串比较
 * @param s1,s2 两个待比较字符串
 * @retval 结果
 *	=0 字符串相等
 *	>0 s1比s2字符多或同下标字符靠后
 *	<0 反之
 @param isCaseSensitive 是否大小写敏感
*/
int strcmp(char* s1,char* s2,bool isCaseSensitive=true);

/**
 * 字符串拷贝
 * @param 把from拷贝到to
 * @param maxlen 字符串最大长度
 * 			如果为0，调用mxalen = strlen()
 * @note 拷贝结束条件是from[n]=0
 * @return strlen(from)
 */
size_t strcpy(char* to,char* from,size_t maxlen=0);

/**
 * 从字符串开头解析整数
 * @param pc 字符串
 * @return 那个整数（找不到为0）
 */
int parseNum(const char* pc);

/**
 * 返回字符串是否为空：
 * 	字符串指针=0
 * 	或字符串首字符=0
 */
bool isEmpty(char* str);

/**
 * 获取有彩色指令字符串长度，计算长度时将不考虑彩色指令的长度，\0为结尾
 * @param str 字符串指针
 * @retval 长度
 * @锅:要是出错了,就是xy的锅
 */
int colorStrlen(char* str);

