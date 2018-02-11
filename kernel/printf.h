#pragma once

#include "types.h"

#define tab_size (8)

enum TextColor
{
	BLACK = 0b0000,
	DARKGRAY = BLACK + 0b1000,

	RED = 0b0100,
	LIGHTRED = RED + 0b1000,
	TOMATO = LIGHTRED,

	GREEN = 0b0010,
	LIGHTGREEN = GREEN + 0b1000,
	LAWNGREEN = LIGHTGREEN,

	BLUE = 0b0001,
	SKYBLUE = BLUE + 0b1000,
	LIGHTBLUE = SKYBLUE,

	ORANGE = 0b0110,
	YELLOW = ORANGE + 0b1000,

	PURPLE = 0b0101,
	PINK = PURPLE + 0b1000,

	CYAN = 0b0011,
	LIGHTCYAN = CYAN + 0b1000,

	WHITE = 0b0111,
	LIGHTGRAY = WHITE,
	LIGHTWHITE = WHITE + 0b1000
};

enum BgColor
{
	bgBLACK = 0b0000 << 4,
	bgRED = 0b0100 << 4,
	bgGREEN = 0b0010 << 4,
	bgBLUE = 0b0001 << 4,
	bgORANGE = 0b0110 << 4,
	bgPURPLE = 0b0101 << 4,
	bgCYAN = 0b0011 << 4,
	bgWHITE = 0b0111 << 4
};

#define makeColor(textcolor, bgcolor) ((textcolor) + (bgcolor))

#define defaultTextColor (WHITE)
#define defaultBgColor (bgBLACK)
const byte defaultColor = makeColor(defaultTextColor, defaultBgColor);

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
 * @param  blink: 是否闪烁（注：在某些显卡上（比如qemu模拟出来的），blink表示是否高亮背景）
 */
void setTerminalColor(TextColor tc, BgColor bc = defaultBgColor, bool blink = false);

/**
 * 另一种方式，直接传入color字节
 */
void setTerminalColorByte(byte c);

/**
 * 设定指针位置
 * @param x,y设定的指针位置
 */
void setCursor(u8 x, u8 y);

/** 
 * 控制台打印单个字符
 * @param  c: 那个字符
 */
void printChar(char c);

/** 
 * 控制台打印字符串
 * @param  str: 字符串
 * @param  maxlen: 字符串最大长度，同时也是补足到的长度；默认len=0，表示len=strlen(str)
 * @param  bFill: 是否补足长度到len
 */
void printStr(char *str, int maxlen = 0, bool bFill = false);

/**
 * 标准库中的没有做attr的printf
 * @param  ...: 填充参数
 * 		1.特别地%f和%lf一定要传入double值
 * 		2.%x打印十六进制，%b打印十六进制。
 * 		默认都是一个字节，可以后面加d|ld|lld改变大小
 * @retval 返回使用了几个可变参数
 */
int printf(const char *const format, ...) __attribute__((format(printf, 1, 2)));

/** 
 * 可以打印颜色的printf
 * @param  format: 格式字符串
 * 			其中${color}表示颜色
 */
int cprintf(const char *format, ...) __attribute__((format(printf, 1, 2)));

/** 
 * 控制输出光标
 * @author wxy
 */
void setXY(const int newx, const int newy);


