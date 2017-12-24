#include "map.h"
#include "types.h"
#include "stdlib.h"

VALUE getValue(KEY key, KEY keys[], VALUE values[], int size, bool isCaseSensitive)
{
	//如果size=-1，自动获取keys长度为size
	if (size < 0)
	{
		for (size = 0; keys[size] != 0; size++)
			;
	}

	for (int i = 0; i < size; i++)
	{
		if (strcmp(keys[i], key, isCaseSensitive) == 0)
		{
			return values[i];
		}
	}
	return (VALUE)-1;
}