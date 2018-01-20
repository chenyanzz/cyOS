#include "stdarg.h"
#include "printf.h"
#include "sprintf.h"
#include "stdlib.h"
#include "asm.h"
#include "map.h"
#include "interrupt/irq.h"

static char x, y; //屏幕光标位置

static char color; //当前颜色设置

const int width = 80;
const int height = 25;

extern output_attrbutes attr;

//显存里字符结构体
struct character
{
	char text;
	char color;
};

struct character *screen[80]; //[40][80]{text,color} 字符显存

char *color_key[] =
	{
		"BLACK", "DARKGRAY", "RED", "LIGHTRED", "TOMATO", "GREEN", "LIGHTGREEN", "LAWNGREEN", "BLUE", "SKYBLUE", "LIGHTBLUE", "ORANGE", "YELLOW", "PURPLE", "PINK", "CYAN", "LIGHTCYAN", "WHITE", "LIGHTGRAY", "LIGHTWHITE",

		"bgBLACK", "bgRED", "bgGREEN", "bgBLUE", "bgORANGE", "bgPURPLE", "bgCYAN", "bgWHITE",

		"normal", "defult", "default", 0}; //以0结尾

char color_value[] =
	{
		BLACK, DARKGRAY, RED, LIGHTRED, TOMATO, GREEN, LIGHTGREEN, LAWNGREEN, BLUE, SKYBLUE, LIGHTBLUE, ORANGE, YELLOW, PURPLE, PINK, CYAN, LIGHTCYAN, WHITE, LIGHTGRAY, LIGHTWHITE,

		bgBLACK, bgRED, bgGREEN, bgBLUE, bgORANGE, bgPURPLE, bgCYAN, bgWHITE,

		defaultColor, defaultColor, defaultColor};

extern struct output_attrbutes attr;

void setTerminalColor(TextColor tc, BgColor bc, bool blink)
{
	color = makeColor(tc, bc) | (blink << 7);
}

void setTerminalColorByte(byte c)
{
	color = c;
}

bool init_terminal()
{
	//获取当前光标位置
	x = *p_cursor_x;
	y = *p_cursor_y;
	setTerminalColor(defaultTextColor, defaultBgColor, false);
	//填补screen数组
	screen[0] = (struct character *)p_firstChar;
	for (int i = 1; i < height; i++)
	{
		screen[i] = screen[i - 1] + width;
	}
	return true;
}

//清屏
void cls()
{
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			screen[j][i].text = ' ';
		}
	}
	x = 0;
	y = 0;
}

//控制台内容整体上移一行
void lineup()
{
	//将这一行数据挪到上一行
	for (int i = 1; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			screen[i - 1][j] = screen[i][j];
		}
	}

	//屏幕上39行补空格
	for (int i = 0; i < width; i++)
	{
		screen[height - 1][i].text = ' ';
	}
}

//判断光标行溢出则换行，列溢出则上滚
void checkXY()
{
	if (x >= width)
	{
		x = 0;
		y++;
	}
	if (y >= height)
	{
		y = height - 1;
		lineup();
	}
}

void setCursor(u8 nx, u8 ny)
{
	x = nx;
	y = ny;
	checkXY();

	//计算光标的线性位置
	u16 cursor_pos = y * 80 + x;

	//告诉地址寄存器要接下来要使用14号寄存器
	outb(0x03d4, 14);

	//向光标位置高位寄存器写入值
	outb(0x03d5, (cursor_pos >> 8) & 0xff);

	//告诉地址寄存器要接下来要使用15号寄存器
	outb(0x03d4, 15);

	//向光标位置高位寄存器写入值
	outb(0x03d5, cursor_pos & 0xff);
}

void printChar(char ch)
{
	close_int();
	
	if (ch == '\n') //换行回车
	{
		y++;
		x = 0;
	}
	else if (ch == '\r') //回车
	{
		x = 0;
	}
	else if (ch == '\b') //退格
	{
		x--;
		if (x < 0)
		{
			x = width - 1;
			y--;
			if (y < 0)
				y = 0;
		}
		screen[y][x].text = ' ';
	}
	else if (ch == '\t') //制表
	{
		x = (x / tab_size + 1) * tab_size;
	}
	else if (ch == '\f') //换页
	{
		cls();
	}
	else if (ch == '\r') //回行首
	{
		x = 0;
	}
	else if (ch == '\v') //垂直制表
	{
		//打印机才有用..
	}
	else
	{
		screen[y][x].text = ch;
		screen[y][x].color = color;
		x++;
	}
	setCursor(x, y);

	start_int();
}

void printStr(char *str, bool bStrip = false, int maxlen = 0, bool bFill = true)
{
	int len = strlen(str);
	if(bStrip)
	{
		len = (len > maxlen ? maxlen : len);
	}
	int i;
	for (i = 0; i < len; i++)
	{
		printChar(str[i]);
	}
	if (bStrip&&bFill)
	{
		for (; i < maxlen; i++)
		{
			printChar(' ');
		}
	}
}

void parseColor(const char *&pc)
{
	//检测当前字符是否是${
	if (*pc == '$')
	{
		char buf[30];
		strFill(buf, 0, 30);
		char *buf2 = 0;
		pc++;
		if (*pc == '$')
		{
			printChar(*pc);
		}
		else if (*pc == '{')
		{
			pc++;
			int i;
			//把{}间的部分存入buf
			for (i = 0; *pc != '}'; i++)
			{
				buf[i] = *pc;
				pc++;
			}
			pc++;
			buf[i] = '\0';
			buf2 = 0;
			//判断buf是不是两个颜色用|分隔
			for (int j = 0; j < i; j++)
			{
				if (buf[j] == '|')
				{
					buf[j] = '\0';
					buf2 = buf + j + 1;
					break;
				}
			}

			//比较buf与各种颜色
			byte tc = defaultTextColor;
			byte bc = defaultBgColor;
			if (!isEmpty(buf))
			{
				tc = getValue(buf, color_key, color_value, -1, false);
			}
			if ((!isEmpty(buf2)) && buf2[0] != '}')
			{
				bc = getValue(buf2, color_key, color_value, -1, false);
			}
			color = makeColor(tc, bc);
		}
	}
}

// int cprintf(const char *format, ...)
// {
// 	va_list vl;
// 	va_start(vl, format);

// 	char buf[1000];
// 	int r = sprintf(buf, format, vl);
// 	printStr(buf);
// 	return r;
// }

int printf(const char *format, ...)
{
	//准备可变参数
	va_list vl;
	va_start(vl, format);

	int params = 0;

	char buf[100];

	const char *pc = format;
	while (*pc != 0)
	{
		//判断是否是设置颜色的${。。。}
		parseColor(pc);

		//如果不是%则正常的输出
		if (*pc != '%')
		{
			printChar(*pc);
			pc++;
			continue;
		}

		//判断%后的字符是什么
		pc++;

		//%[sign][width][.dp]  捕获%后面属性
		parseAttr(pc, vl);
		bool bPrintBuf = false;
		params++;
		switch (*pc)
		{
		case 'c':
			printChar(va_arg(vl, char));
			break;
		case 's':
			printStr(va_arg(vl, char *));
			break;
		case 'u':
			sprintUInt(va_arg(vl, char *), buf);
			bPrintBuf = true;
			break;
		case 'd':
			sprintInt(va_arg(vl, int), buf);
			bPrintBuf = true;
			break;
		case 'x':
			sprintHex(va_arg(vl, unsigned int), buf, false);
			bPrintBuf = true;
			break;
		case 'X':
			sprintHex(va_arg(vl, unsigned int), buf, true);
			bPrintBuf = true;
			break;
		case 'f':
			sprintDouble(va_arg(vl, double), attr.dp, buf);
			bPrintBuf = true;
			break;
		case 'l':
			pc++;
			switch (*pc)
			{
			case 'u':
				sprintUInt(va_arg(vl, unsigned long), buf);
				bPrintBuf = true;
				break;
			case 'd':
				sprintInt(va_arg(vl, long), buf);
				bPrintBuf = true;
				break;
			case 'x':
				sprintHex(va_arg(vl, unsigned long), buf, false);
				bPrintBuf = true;
				break;
			case 'X':
				sprintHex(va_arg(vl, unsigned long), buf, true);
				bPrintBuf = true;
				break;
			case 'f':
				sprintDouble(va_arg(vl, double), attr.dp, buf);
				bPrintBuf = true;
				break;
			case 'l':
				pc++;
				switch (*pc)
				{
				case 'u':
					sprintUInt(va_arg(vl, unsigned long long), buf);
					bPrintBuf = true;
					break;
				case 'd':
					sprintInt(va_arg(vl, long long), buf);
					bPrintBuf = true;
					break;
				case 'x':
					sprintHex(va_arg(vl, unsigned long long), buf, false);
					bPrintBuf = true;
					break;
				case 'X':
					sprintHex(va_arg(vl, unsigned int), buf, true);
					bPrintBuf = true;
					break;
				}
				break;
			}
			break;
		case 'h':
			pc++;
			switch (*pc)
			{
			case 'u':
				sprintUInt(va_arg(vl, unsigned short), buf);
				bPrintBuf = true;
				break;
			case 'd':
				sprintInt(va_arg(vl, short), buf);
				bPrintBuf = true;
				break;
			case 'x':
				sprintHex(va_arg(vl, unsigned short), buf, false);
				bPrintBuf = true;
				break;
			case 'X':
				sprintHex(va_arg(vl, unsigned short), buf, true);
				bPrintBuf = true;
				break;
			case 'h':
				pc++;
				switch (*pc)
				{
				case 'u':
					sprintUInt(va_arg(vl, unsigned char), buf);
					bPrintBuf = true;
					break;
				case 'd':
					sprintInt(va_arg(vl, char), buf);
					bPrintBuf = true;
					break;
				case 'x':
					sprintHex(va_arg(vl, unsigned char), buf, false);
					bPrintBuf = true;
					break;
				case 'X':
					sprintHex(va_arg(vl, unsigned char), buf, true);
					bPrintBuf = true;
					break;
				}
				break;
			}
			break;
		default:
			printChar(*pc);
			params--;
		}
		if (bPrintBuf)
		{
			printStr(buf);
			buf[0] = 0;
		}
		pc++;
	}
	va_end(vl);
	return params;
}

void setXY(const int newx, const int newy)
{
	close_int();

	x = newx;
	y = newy;

	start_int();
}