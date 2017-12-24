#include "stdarg.h"
#include "printf.h"
#include "stdlib.h"
#include "asm.h"

static char x,y;//屏幕光标位置

static char color;//当前颜色设置

const int width = 80;
const int height = 25;

//显存里字符结构体
struct character
{
	char text;
	char color;
};

struct character* screen[80];//[40][80]{text,color} 字符显存

const struct character SPACE = {' ',defaultColor};//空格

void setTerminalColor(TextColor tc, BgColor bc, bool blink)
{
	color = makeColor(tc,bc)|(blink<<7);
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
	setTerminalColor(defaultTextColor,defaultBgColor,false);
	//填补screen数组
	screen[0]=(struct character*)p_firstChar;
	for(int i=1;i<height;i++)
	{
		screen[i]=screen[i-1]+width;
	}
	return true;
}

//清屏
void cls()
{
	for(int j=0;j<height;j++)
	{
		for(int i=0;i<width;i++)
		{
			screen[j][i]=SPACE;
		}
	}
	x=0;
	y=0;
}

//控制台内容整体上移一行
void lineup()
{
	//将这一行数据挪到上一行
	for(int i=1;i<height;i++)
	{
		for(int j=0;j<width;j++)
		{
			screen[i-1][j]=screen[i][j];
		}
	}

	//屏幕上39行补空格
	for(int i=0;i<width;i++)
	{
		screen[height-1][i]=SPACE;
	}
}

//判断光标行溢出则换行，列溢出则上滚
void checkXY()
{
	if(x>=width)
	{
		x=0;
		y++;
	}
	if(y>=height)
	{
		y=height-1;
		lineup();
	}
}

void printChar(char ch)
{
	if(ch=='\n')//换行回车
	{
		y++;
		x=0;
		checkXY();
	}else if(ch=='\r')//回车
	{
		x=0;
	}else if(ch=='\b')//退格
	{
		x--;
		if(x<0)
		{
			x=width-1;
			y--;
			if(y<0)y=0;
		}
	}else if(ch=='\t')//制表
	{
		if(x%tab_size==0)return;
		x=(x/tab_size+1)*tab_size;
		checkXY();
	}else if(ch=='\v'||ch=='\f')//换页||垂直制表
	{
		//打印机才有用..
	}else
	{
		screen[y][x].text=ch;
		screen[y][x].color = color;
		x++;
		checkXY();
	}
}

void printStr(char* str)
{
	int i = 0;
	while (str[i] != '\0')
	{
		printChar(str[i]);
		i++;
	}
}

void printInt(long long val)
{
	if (val < 0)
	{
		printChar('-');
		val = -val;
	}
	printUInt(val);
}

void printUInt(unsigned long long val)
{
	if (val == 0)printChar('0');
	char str[30];
	strFill(str, 0, 30);
	int pos;
	for (pos = 0; val != 0; pos++)
	{
		str[pos] = '0' + val % 10;
		val /= 10;
	}
	strReverse(str, pos);
	printStr(str);
}

void printHex(unsigned long long val)
{
	// printStr("0x");
	const char nums[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
	if (val == 0)printChar('0');
	char str[30];
	strFill(str, 0, 30);
	int pos;
	for (pos = 0; val != 0; pos++)
	{
		str[pos] = nums[val % 16];
		val >>= 4;
	}
	strReverse(str, pos);
	printStr(str);
}

void printBinary(unsigned long long val)
{
	printStr("0b");
	if (val == 0)printChar('0');
	char str[64];
	strFill(str, 0, 32);
	int pos;
	for (pos = 0; val != 0; pos++)
	{
		str[pos] = '0' + val % 2;
		val >>= 1;
	}
	strReverse(str, pos);
	printStr(str);
}

void printDouble(double val, int dp)
{
	int intval = val;
	printInt(intval);
	val -= intval;
	if (dp < 1)
	{
		return;
	}
	printChar('.');
	int i;
	for (i = 0; i < dp; i++)
	{
		val *= 10;
		printChar('0' + ((int)val % 10));
	}
}

struct output_attrbutes
{
	char sign;//符号(符号正号空格等)——默认0
	unsigned char width;//变量整体宽度——默认-1（无限）
	unsigned char dp;//小数点后位数——默认2
};
static struct  output_attrbutes attr;

/**
* 读取输出属性到结构体
* @param ppc 字符串指针
* @param vl 可变参数列表，用于读取*代替属性值
*/
void parseAttr(const char** ppc, va_list vl)
{
	//sign
	if(**ppc=='-'||**ppc=='+'||**ppc==' ')
	{
		attr.sign = **ppc;
		*ppc++;
	}else attr.sign = 0;

	//width.dp
	attr.width = 0;
	while (**ppc >= '0' && **ppc <= '9')
	{
		attr.width *= 10;
		attr.width += **ppc - '0';
		*ppc++;
	}
	if (**ppc != '.')
	{
		return;
	}
	attr.dp = 0;
	while (**ppc >= '0' && **ppc <= '9')
	{
		attr.dp *= 10;
		attr.dp += **ppc - '0';
		*ppc++;
	}
	if (attr.dp == 0)attr.dp = 2;
}

int printf(const char* format,...)
{
	//准备可变参数
	va_list vl;
	va_start(vl, format);

	int params=0;

	const char* pc = format;
	int i=0;
	while (*pc != 0)
	{
		//如果不是%则正常的输出
		if(*pc!='%')
		{
			printChar(*pc);
			pc++;
			continue;
		}

		//判断%后的字符是什么
		pc++;
		parseAttr(&pc,vl);

		if(*pc=='*')
		{
			while(*pc=='*')pc++;
			continue;
		}
		if(*pc=='$')
		{
			printChar(*pc);
			pc++;
			continue;
		}
		params++;
		switch(*pc)
		{
		case 'c':
			printChar(va_arg(vl, char));
			break;
		case 's':
			printStr(va_arg(vl, char*));
			break;
		case 'u':
			printStr(va_arg(vl, char*));
			break;
		case 'd':
			printInt(va_arg(vl, int));
			break;
		case 'f':
			printDouble(va_arg(vl, double),attr.dp);
			break;
		case 'l':
			pc++;
			switch(*pc)
			{
			case 'u':
				printUInt(va_arg(vl, unsigned long));
				break;
			case 'd':
				printInt(va_arg(vl, long));
				break;
			case 'f':
				printDouble(va_arg(vl, double),2);
				break;
			case 'l':
				pc++;
				switch(*pc)
				{
				case 'u':
					printUInt(va_arg(vl, unsigned long long));
					break;
				case 'd':
					printInt(va_arg(vl, long long));
					break;
				}
				break;
			}
			break;
		case 'x':
			pc++;
			switch(*pc)
			{
			case 'c':
				printHex(va_arg(vl, char));
				break;
			case 'd':
				printHex(va_arg(vl, int));
				break;
			case 'l':
				pc++;
				switch(*pc)
				{
				case 'd':
					printHex(va_arg(vl, long));
					break;
				case 'l':
					pc++;
					switch(*pc)
					{
					case 'd':
						printHex(va_arg(vl, long long));
						break;
					}
				}
				break;
			default:
				printHex(va_arg(vl, char));
			}
			break;
		case 'b':
			pc++;
			switch(*pc)
			{
			case 'c':
				printBinary(va_arg(vl, char));
				break;
			case 'd':
				printBinary(va_arg(vl, int));
				break;
			case 'l':
				pc++;
				switch(*pc)
				{
				case 'd':
					printBinary(va_arg(vl, long));
					break;
				case 'l':
					pc++;
					switch(*pc)
					{
					case 'd':
						printBinary(va_arg(vl, long long));
						break;
					}
				}
				break;
			default:
				printBinary(va_arg(vl, char));
			}
			break;
		default:
			printChar(*pc);
			params--;
		}
		pc++;
	}
	va_end(vl);
	return params;
}

void setXY(const int newx, const int newy)
{
	x = newx;
	y = newy;
}