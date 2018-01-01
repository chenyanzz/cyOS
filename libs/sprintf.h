#ifndef SPRINTF_H
#define SPRINTF_H

#include "types.h"
#include "stdarg.h"
/******************************************************************************
 * 处理把格式字符串输出到buf中
 * @return 全部函数返回值是写的字符长度（不算上结尾0）
 * @author cy
******************************************************************************/

#define MAX_DP 50

struct output_attrbutes
{
	char sign;			 //符号(符号正号空格等)——默认0
	unsigned char width; //变量整体宽度——默认-1（无限）
	unsigned char dp;	//小数点后位数——默认2
};

/** 
 * 打印不同类型数字（十进制）
 * @param  val: 数值 
 */
int sprintInt(long long val, char *buf);
int sprintUInt(unsigned long long val, char *buf);

/** 
 * 打印十六进制值||二进制
 * @param  val: 数值
 */
int sprintHex(unsigned long long val,char *buf, bool isCapital = true);
int sprintOct(unsigned long long val, char *buf);
int sprintBinary(unsigned long long val, char *buf);


/** 
 * 控制台打印浮点数
 * @param  val: 值
 * @param  dp: 小数点后长度 注意dp要不大于MAX_DP
 */
int sprintDouble(double val, int dp, char *buf);

/**
* 读取输出属性到结构体
* @param ppc 字符串指针
* @param vl 可变参数列表，用于读取*代替属性值
*/
void parseAttr(const char *&pc, va_list vl);

/** 
 * 实现标准库中printf
 * @param  format: 格式字符串
 * @param  ...: 填充参数
 * 		1.特别地%f和%lf一定要传入double值
 * 		2.%x打印十六进制，%b打印十六进制。
 * 		默认都是一个字节，可以后面加d|ld|lld改变大小
 */
int sprintf(char *buf, const char *const format, ...) __attribute__((format(printf, 2, 3)));

#endif //SPRINTF_H
