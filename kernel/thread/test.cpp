#include "thread.h"
#include "printf.h"

void t1()
{
	while (1)
	{
		for (int i = 0; i < 7; i++)
			printChar('1');
		switch_to_t2();
	}
}

void t2()
{
	while (1)
	{
		for (int i = 0; i < 7; i++)
			printChar('2');
		switch_to_t1();
	}
}