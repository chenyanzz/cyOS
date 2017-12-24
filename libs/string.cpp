void swap(char *a, char *b)
{
	char buf = *a;
	*a = *b;
	*b = buf;
}

void strReverse(char *str, int len)
{
	int lpos = 0, rpos = len - 1;
	char buf;
	while (lpos < rpos)
	{
		buf = str[lpos];
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

int strlen(char *str)
{
	int i;
	for (i = 0; str[i] != 0; i++)
		;
	return i;
}

int strcmp(char *s1, char *s2)
{
	int i;
	for (i = 0; s1[i] == s2[i]; i++)
		;
	return s1[i] - s2[i];
}