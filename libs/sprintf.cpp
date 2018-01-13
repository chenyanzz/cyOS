#include "stdarg.h"
#include "sprintf.h"
#include "stdlib.h"

output_attrbutes attr;

int sprintInt(long long val, char *buf)
{
	int len = 1;
	switch (attr.sign)
	{
	case '+':
		buf[0] = val > 0 ? '+' : '-';
		buf++;
		break;
	case ' ':
		buf[0] = val > 0 ? ' ' : '-';
		buf++;
		break;
	default:
		len = 0;
	}
	len += sprintUInt(val, buf);
	return len;
}

int sprintUInt(unsigned long long val, char *buf)
{

	int pos = 0;
	do
	{
		buf[pos] = '0' + val % 10;
		val /= 10;
		pos++;
	} while (val != 0);

	strReverse(buf, pos);
	buf[pos] = 0;

	return pos;
}

int sprintOct(unsigned long long val, char *buf)
{
	int pos = 0;
	do
	{
		buf[pos] = '0' + val % 8;
		val >>= 3;
		pos++;
	} while (val != 0);

	strReverse(buf, pos);
	buf[pos] = 0;

	return pos;
}

int sprintHex(unsigned long long val, char *buf, bool isCapital)
{
	const char nums[2][16] = {
		{'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'},
		{'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'}};

	int pos = 0;
	do
	{
		buf[pos] = nums[isCapital][val % 16];
		val >>= 4;
		pos++;
	} while (val != 0);

	strReverse(buf, pos);
	buf[pos] = 0;

	return pos;
}

int sprintBinary(unsigned long long val, char *buf)
{
	int pos = 0;
	do
	{
		buf[pos] = '0' + val % 2;
		val >>= 1;
		pos++;
	} while (val != 0);

	strReverse(buf, pos);
	buf[pos] = 0;

	return pos;
}

int sprintDouble(double val, int dp, char *buf)
{
	dp %= MAX_DP;

	int intval = val;

	int len = sprintInt(intval, buf);
	buf += len;

	val -= intval;
	if (dp < 1)
	{
		return len;
	}

	buf[0] = '.';
	int pos;
	for (pos = 1; pos < dp; pos++)
	{
		pos *= 10;
		int figure = ((int)val) % 10;
		buf[pos] = '0' + figure;
	}
	buf[pos] = 0;
	return len + pos;
}

void parseAttr(const char *&pc, va_list vl)
{
	//sign
	if (*pc == '+' || *pc == ' ' || *pc == '#')
	{
		attr.sign = *pc;
		pc++;
	}
	else
	{
		attr.sign = 0;
	}

	//width.dp
	attr.width = 0;
	while (*pc >= '0' && *pc <= '9')
	{
		attr.width *= 10;
		attr.width += *pc - '0';
		pc++;
	}
	if (*pc != '.')
	{
		return;
	}
	attr.dp = 0;
	while (*pc >= '0' && *pc <= '9')
	{
		attr.dp *= 10;
		attr.dp += *pc - '0';
		pc++;
	}
	if (attr.dp == 0)
		attr.dp = 2;
}

// int sprintf(char *buf, const char *format, ...)
// {
// 	//准备可变参数
// 	va_list vl;
// 	va_start(vl, format);

// 	int pos = 0;
// 	int params = 0;
// 	const char *pc = format;
// 	char *buf_start = buf;
// 	while (*pc != 0)
// 	{
// 		//如果不是%则正常的输出
// 		if (*pc != '%')
// 		{
// 			buf[pos++] = *pc;
// 			pc++;
// 			continue;
// 		}

// 		//判断%后的字符是什么
// 		pc++;

// 		//%[sign][width][.dp]  捕获%后面属性
// 		parseAttr(pc, vl);

// 		params++;
// 		switch (*pc)
// 		{
// 		case 'c':
// 			*buf = va_arg(vl, char);
// 			buf++;
// 			break;
// 		case 's':
// 			char *pfrom = va_arg(vl, char *);
// 			if (attr.width != 0)
// 			{
// 				attr.width = strlen(pfrom);
// 			}
// 			strcpy(buf, pfrom, attr.width);
// 			buf += attr.width;
// 			break;
// 		case 'u':
// 			buf += sprintUInt(va_arg(vl, unsigned int), buf);
// 			break;
// 		case 'd':
// 			buf += sprintInt(va_arg(vl, int), buf);
// 			break;
// 		case 'x':
// 			buf += sprintHex(va_arg(vl, unsigned int), buf, false);
// 			break;
// 		case 'X':
// 			buf += sprintHex(va_arg(vl, unsigned int), buf, true);
// 			break;
// 		case 'f':
// 			buf += sprintDouble(va_arg(vl, double), attr.dp, buf);
// 			break;
// 		case 'l':
// 			pc++;
// 			switch (*pc)
// 			{
// 			case 'u':
// 				buf += sprintUInt(va_arg(vl, unsigned long), buf);
// 				break;
// 			case 'd':
// 				buf += sprintInt(va_arg(vl, long), buf);
// 				break;
// 			case 'x':
// 				buf += sprintHex(va_arg(vl, unsigned long), buf, false);
// 				break;
// 			case 'X':
// 				buf += sprintHex(va_arg(vl, unsigned long), buf, true);
// 				break;
// 			case 'f':
// 				buf += sprintDouble(va_arg(vl, double), 2, buf);
// 				break;
// 			case 'l':
// 				pc++;
// 				switch (*pc)
// 				{
// 				case 'u':
// 					buf += sprintUInt(va_arg(vl, unsigned long long), buf);
// 					break;
// 				case 'd':
// 					buf += sprintInt(va_arg(vl, long long), buf);
// 					break;
// 				case 'x':
// 					buf += sprintHex(va_arg(vl, unsigned long long), buf, false);
// 					break;
// 				case 'X':
// 					buf += sprintHex(va_arg(vl, unsigned int), buf, true);
// 					break;
// 				}
// 				break;
// 			}
// 			break;
// 		case 'h':
// 			pc++;
// 			switch (*pc)
// 			{
// 			case 'u':
// 				buf += sprintUInt(va_arg(vl, unsigned short), buf);
// 				break;
// 			case 'd':
// 				buf += sprintInt(va_arg(vl, short), buf);
// 				break;
// 			case 'x':
// 				buf += sprintHex(va_arg(vl, unsigned short), buf, false);
// 				break;
// 			case 'X':
// 				buf += sprintHex(va_arg(vl, unsigned short), buf, true);
// 				break;
// 			case 'h':
// 				pc++;
// 				switch (*pc)
// 				{
// 				case 'u':
// 					buf += sprintUInt(va_arg(vl, unsigned char), buf);
// 					break;
// 				case 'd':
// 					buf += sprintInt(va_arg(vl, char), buf);
// 					break;
// 				case 'x':
// 					buf += sprintHex(va_arg(vl, unsigned char), buf, false);
// 					break;
// 				case 'X':
// 					buf += sprintHex(va_arg(vl, unsigned char), buf, true);
// 					break;
// 				}
// 				break;
// 			}
// 			break;
// 		default:
// 			buf[pos++] = *pc;
// 			params--;
// 		}
// 		pc++;
// 	}
// 	va_end(vl);
// 	*buf = 0;
// 	return buf - buf_start;
// }
