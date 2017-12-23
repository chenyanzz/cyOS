#ifndef STD_OUT_H
#define STD_OUT_H

#include "types.h"

#define tab_size (8)


enum TextColor
{
	BLACK		= 0b0000,
	DARKGRAY	= BLACK+0b1000,
	
	RED			= 0b0100,
	LIGHTRED	= RED+0b1000,
	TOMATO		= LIGHTRED,

	GREEN		= 0b0010,
	LIGHTGREEN	= GREEN+0b1000,
	LAWNGREEN	= LIGHTGREEN,

	BLUE		= 0b0001,
	SKYBLUE		= BLUE+0b1000,
	LIGHTBLUE	= SKYBLUE,
	
	ORANGE		= 0b0110,
	YELLOW		= ORANGE+0b1000,

	PURPLE		= 0b0101,
	PINK		= PURPLE+0b1000,

	CYAN		= 0b0011,
	LIGHTSYAN	= CYAN+0b1000,

	WHITE		= 0b0111,
	LIGHTGRAY	= WHITE,
	LIGHTWHITE	= WHITE+0b1000
};

enum BgColor
{
	bgBLACK		= 0b0000,
	bgRED		= 0b0100,
	bgGREEN		= 0b0010,
	bgBLUE		= 0b0001,
	bgORANGE	= 0b0110,
	bgPURPLE	= 0b0101,
	bgCYAN		= 0b0011,
	bgWHITE		= 0b0111
};

#define makeColor(textcolor,bgcolor) ((textcolor)+((bgcolor)<<4))

#define defaultTextColor (WHITE)
#define defaultBgColor (bgBLACK)
const byte defaultColor = makeColor(defaultTextColor,defaultBgColor);

/** 
 * 初始化控制台
 */
bool init_terminal();

/** 
 * 清空命令行内容
 */
void cls();

/** 
 * 设置命令行颜色属性
 * @param  tc: 前景字符色
 * @param  bc: 背景色
 * @param  blink: 是否闪烁
 */
void setTerminalColor(TextColor tc, BgColor bc=defaultBgColor, bool blink=false);

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

/** 
 * 控制输出光标
 * @author wxy
 */
void setXY(const int newx, const int newy);
#endif
