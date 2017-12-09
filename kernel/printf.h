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
 * @brief  cy
 * @note   清空命令行内容
 * @retval None
 */
void cls();

/** 
 * @brief  cy
 * @note   设置命令行字符前景色与背景色
 * @param  a: 8位分别 lrgb(text) lrgb (bg)		l为高亮
 * @retval None
 */
void setTerminalColorAttr(char a);

/** 
 * @brief  cy
 * @note   控制台打印单个字符
 * @param  c: 那个字符
 * @retval None
 */
void printChar(char c);

/** 
 * @brief  cy
 * @note   控制台打印字符串
 * @param  str: 字符串
 * @retval None
 */
void printStr(char* str);

/** 
 * @brief  cy
 * @note   打印不同类型数字
 * @param  val: 数值 
 * @retval None
 */
void printUInt(unsigned int val);
void printInt(int val);
void printLong(long val);
void printULong(unsigned long val);
void printLongLong(long long val);
void printULongLong(unsigned long long val);

/** 
 * @brief  cy
 * @note   控制台打印浮点数
 * @param  val: 值
 * @param  dp: 小数点后长度
 * @retval None
 */
void printDouble(double val, int dp);

/** 
 * @brief  cy
 * @note   实现标准库中printf
 * @param  format: 格式字符串
 * @param  ...: 填充参数
 * @retval None
 */
void printf(const char* const format, ...);

#endif
