#include "stdarg.h"
#include "printf.h"
#include "stringUtils.h"

static char x,y;//屏幕光标位置
static char TextColor;//当前颜色设置

//显存里字符结构体
struct character
{
	char text;
	char color;
};

struct character* screen[40];//[40][80]{text,color} 字符显存
//struct character screen[40][80];//调试版*数组显存
const struct character SPACE = {' ',0};//空格

void setTerminalColorAttr(char a)
{
	TextColor = a;
}

void init_terminal()
{
	//获取当前光标位置
	x = *p_cursor_x;
	y = *p_cursor_y;
	TextColor = defult_attr;
	//填补screen数组
	screen[0]=(struct character*)p_firstChar;
	for(int i=1;i<40;i++)
	{
		screen[i]=screen[i-1]+80;
	}
}

//清屏
void cls()
{
	for(int j=0;j<40;j++)
	{
		for(int i=0;i<160;i++)
		{
			screen[j][i]=SPACE;
		}
	}
}

//控制台内容整体上移一行
void lineup()
{
	//将这一行数据挪到上一行
	for(int i=1;i<40;i++)
	{
		for(int j=0;j<80;j++)
		{
			screen[i-1][j]=screen[i][j];
		}
	}

	//屏幕上39行补空格
	for(int i=0;i<80;i++)
	{
		screen[39][i]=SPACE;
	}
}

//判断光标行溢出则换行，列溢出则上滚
void checkXY()
{
	if(x>=80)
	{
		x=0;
		y++;
	}
	if(y>=40)
	{
		y=39;
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
			x=79;
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
		screen[y][x].color = TextColor;
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

void printInt(int val)
{
	printLongLong(val);
}

void printUInt(unsigned int val)
{
	printULongLong(val);
}

void printLong(long val)
{
	printLongLong(val);
}

void printULong(unsigned long val)
{
	printULongLong(val);
}

void printLongLong(long long val)
{
	if (val < 0)
	{
		printChar('-');
		val = -val;
	}
	printULongLong(val);
}

void printULongLong(unsigned long long val)
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

void printf(const char* const format,...)
{
	//准备可变参数
	va_list vl;
	va_start(vl, format);
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
		if(*pc=='%')		printChar('%');
		else if(*pc=='c')	printChar(va_arg(vl, char));
		else if(*pc=='s')	printStr(va_arg(vl, char*));
		else if(*pc=='u')	printUInt(va_arg(vl, unsigned int));
		else if(*pc=='d')	printInt(va_arg(vl, int));
		else if(*pc=='f')	printDouble(va_arg(vl, double),attr.dp);
		else if(*pc=='l')
		{
			pc++;
			if(*pc=='u')		printULong(va_arg(vl, unsigned long));
			else if(*pc=='d')	printLong(va_arg(vl, long));
			else if(*pc=='f')	printDouble(va_arg(vl, double),2);
			else if(*pc=='l')
			{
				pc++;
				if(*pc=='u')		printULongLong(va_arg(vl, unsigned long long));
				else if(*pc=='d')	printLongLong(va_arg(vl, long long));
			}
		}
		else	printChar(*pc);
		pc++;
	}
	va_end(vl);
}