#include "string.h"
#include "math.h"

void swap(char *a, char *b)
{
	char buf = *a;
	*a = *b;
	*b = buf;
}

void strReverse(char *str, int len)
{
	int lpos = 0, rpos = len - 1;
	while (lpos < rpos)
	{
		swap(str + lpos, str + rpos);
		lpos++;
		rpos--;
	}
}

void strFill(char *str, char c, int len)
{
	int i;
	for (i = 0; i < len; i++)
	{
		str[i] = c;
	}
}

int parseNum(const char *pc)
{
	int num = -1;
	while (*pc >= '0' && *pc <= '9')
	{
		num *= 10;
		num += *pc - '0';
		pc++;
	}
	return num;
}

size_t strlen(char *str)
{
	size_t i;
	for (i = 0; str[i] != 0; i++)
		;
	return i;
}
#include "stdio.h"

int strcmp(char *s1, char *s2, bool isCaseSensitive)
{
	int i = 0;
	for (i = 0; (~(((~(s1[i] ^ s2[i])) | ((!isCaseSensitive) << 5)))) == 0; i++)
	{
		if ((s1[i] && s2[i]) == 0)
		{
			break;
		}
	}
	return s1[i] - s2[i];
}

size_t strcpy(char *to, char *from, size_t maxlen)
{
	if (maxlen <= 0)
	{
		maxlen = strlen(from);
	}

	size_t i;
	for (i = 0; ((i < maxlen) && ((to[i] = from[i]) != 0)); i++)
	{

	}

	return i;
}

bool isEmpty(char *str)
{
	return (str == 0) || str[0] == 0;
}

int colorStrlen(char *str)
{
	int i = 0;
	int j = 0;
	for (i = 0; str[i] != '\0'; i++)
	{
		if (str[i] == '$' && str[++i] == '{')
		{
			while (str[++i] != '}')
			{
				continue;
			}
			continue;
		}
		j++;
	}
	return j;
}