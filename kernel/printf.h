#ifndef STD_OUT_H
#define STD_OUT_H

#define tab_size 8
#define defult_attr 7//黑底白字

#include "define.h"

/**
* 初始化控制台
*/
void init_terminal();

/**
* 清空屏幕
*/
void cls();


/**
 * 设置字符前景色与背景色
 */
void setTerminalColorAttr(char a);

/**
* 控制台打印单个字符
* @param c 打印的字符
*/
void printChar(char c);

/**
* 控制台打印字符串
* @param str 字符串
*/
void printStr(char* str);

/**
* 控制台打印无符号整数
* @param val 值
*/
void printUInt(unsigned int val);

/**
* 控制台打印有符号整数
* @param val 值
*/
void printInt(int val);

/**
* 控制台打印有符号长整数
* @param val 值
*/
void printLong(long val);

/**
* 控制台打印无符号长整数
* @param val 值
*/
void printULong(unsigned long val);

/**
* 控制台打印有符号长长整数
* @param val 值
*/
void printLongLong(long long val);

/**
* 控制台打印无符号长长整数
* @param val 值
*/
void printULongLong(unsigned long long val);

/**
* 控制台打印浮点数
* @param val 值
* @param dp 小数点后长度
*/
void printDouble(double val, int dp);

/**
* 控制台打印带格式字符串
* @param format 字符串格式
* @param ... 填充参数
*/
void printf(const char* const format, ...);

#endif