#include "stdarg.h"
#include "stdlib.h"

// int scanf(const char *format, ...)
// {
	// //准备可变参数
	// va_list vl;
	// va_start(vl, format);

	// int params = 0;

	// const char *pc = format;
	// int i = 0;
	// while (*pc != 0)
	// {
	// 	//如果不是%则正常的输出
	// 	if (*pc != '%')
	// 	{
	// 		printChar(*pc);
	// 		pc++;
	// 		continue;
	// 	}

	// 	//判断%后的字符是什么
	// 	pc++;
	// 	parseAttr(&pc, vl);

	// 	if (*pc == '*')
	// 	{
	// 		while (*pc == '*')
	// 			pc++;
	// 		continue;
	// 	}
	// 	if (*pc == '$')
	// 	{
	// 		printChar(*pc);
	// 		pc++;
	// 		continue;
	// 	}
	// 	params++;
	// 	switch (*pc)
	// 	{
	// 	case 'c':
	// 		printChar(va_arg(vl, char));
	// 		break;
	// 	case 's':
	// 		printStr(va_arg(vl, char *));
	// 		break;
	// 	case 'u':
	// 		printStr(va_arg(vl, char *));
	// 		break;
	// 	case 'd':
	// 		printInt(va_arg(vl, int));
	// 		break;
	// 	case 'f':
	// 		printDouble(va_arg(vl, double), attr.dp);
	// 		break;
	// 	case 'l':
	// 		pc++;
	// 		switch (*pc)
	// 		{
	// 		case 'u':
	// 			printUInt(va_arg(vl, unsigned long));
	// 			break;
	// 		case 'd':
	// 			printInt(va_arg(vl, long));
	// 			break;
	// 		case 'f':
	// 			printDouble(va_arg(vl, double), 2);
	// 			break;
	// 		case 'l':
	// 			pc++;
	// 			switch (*pc)
	// 			{
	// 			case 'u':
	// 				printUInt(va_arg(vl, unsigned long long));
	// 				break;
	// 			case 'd':
	// 				printInt(va_arg(vl, long long));
	// 				break;
	// 			}
	// 			break;
	// 		}
	// 		break;
	// 	case 'x':
	// 		pc++;
	// 		switch (*pc)
	// 		{
	// 		case 'c':
	// 			printHex(va_arg(vl, char));
	// 			break;
	// 		case 'd':
	// 			printHex(va_arg(vl, int));
	// 			break;
	// 		case 'l':
	// 			pc++;
	// 			switch (*pc)
	// 			{
	// 			case 'd':
	// 				printHex(va_arg(vl, long));
	// 				break;
	// 			case 'l':
	// 				pc++;
	// 				switch (*pc)
	// 				{
	// 				case 'd':
	// 					printHex(va_arg(vl, long long));
	// 					break;
	// 				}
	// 			}
	// 			break;
	// 		default:
	// 			printHex(va_arg(vl, char));
	// 		}
	// 		break;
	// 	case 'b':
	// 		pc++;
	// 		switch (*pc)
	// 		{
	// 		case 'c':
	// 			printBinary(va_arg(vl, char));
	// 			break;
	// 		case 'd':
	// 			printBinary(va_arg(vl, int));
	// 			break;
	// 		case 'l':
	// 			pc++;
	// 			switch (*pc)
	// 			{
	// 			case 'd':
	// 				printBinary(va_arg(vl, long));
	// 				break;
	// 			case 'l':
	// 				pc++;
	// 				switch (*pc)
	// 				{
	// 				case 'd':
	// 					printBinary(va_arg(vl, long long));
	// 					break;
	// 				}
	// 			}
	// 			break;
	// 		default:
	// 			printBinary(va_arg(vl, char));
	// 		}
	// 		break;
	// 	default:
	// 		printChar(*pc);
	// 		params--;
	// 	}
	// 	pc++;
	// }
	// va_end(vl);
	// return params;
// }