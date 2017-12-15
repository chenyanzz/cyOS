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
 * 清空命令行内容
 */
void cls();

/** 
 * 设置命令行字符前景色与背景色
 * @param  a: 8位分别 lrgb(text) lrgb (bg)		l为高亮
 */
void setTerminalColorAttr(char a);

/** 
 * 控制台打印单个字符
 * @param  c: 那个字符
 */
void printChar(char c);

/** 
 * 控制台打印字符串
 * @param  str: 字符串
 */
void printStr(char* str);

/** 
 * 打印不同类型数字（十进制）
 * @param  val: 数值 
 */
void printInt(long long val);
void printUInt(unsigned long long val);

/** 
 * 打印十六进制值||二进制
 * @param  val: 数值
 */ 
void printHex(unsigned long long val);
void printBinary(unsigned long long val);

/** 
 * 控制台打印浮点数
 * @param  val: 值
 * @param  dp: 小数点后长度
 * @retval None
 */
void printDouble(double val, int dp);

/** 
 * 实现标准库中printf
 * @param  format: 格式字符串
 * @param  ...: 填充参数
 * 		1.特别地%f和%lf一定要传入double值
 * 		2.%x打印十六进制，%b打印十六进制。
 * 		默认都是一个字节，可以后面加d|ld|lld改变大小
 * @retval None
 */
void printf(const char* const format, ...);

#endif
