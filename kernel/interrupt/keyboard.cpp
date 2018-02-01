#include "keyboard.h"
#include "key_map.h"
#include "asm.h"
#include "irq.h"
#include "stdio.h"
#include "stdlib.h"

struct KeyState
{
	bool isShift : 1;
	bool isCtrl : 1;
	bool isAlt : 1;
	bool isCapsLk : 1;
	bool isNumLk : 1;
	bool isSclLk : 1;
} kstat;

//键盘读取到的字符的缓冲区，循环链表
char charbuf[MAX_KEYBUF];

//读位置和写位置
int pRead, pWrite;

bool init_keyboard()
{
	memset(charbuf, 0, sizeof(charbuf));
	pRead = 0;
	pWrite = 0;
	start_irq(KEYBOARD_IRQ);
	return true;
}

extern "C" void key_state_changed()
{
	byte scan_code = inb(0x60);

	bool isDown = !bit(scan_code, 7);
	byte key = scan_code & 0b01111111;

	switch (key)
	{
	case KEY_LEFT_SHIFT:
	case KEY_RIGHT_SHIFT:
		kstat.isShift = isDown;
		break;

	case KEY_ALT:
		kstat.isAlt = isDown;
		break;

	case KEY_CTRL:
		kstat.isCtrl = isDown;
		break;

	case KEY_CAPS_LOCK:
		kstat.isCapsLk = isDown;
		break;

	case KEY_NUM_LOCK:
		kstat.isNumLk = isDown;
		break;

	case KEY_SCROLL_LOCK:
		kstat.isSclLk = isDown;
		break;
	}

	bool isCapital = kstat.isShift ^ kstat.isCapsLk;
	if (keys[key][isCapital] != 0)
	{
		if (isDown)
		{
			write_charbuf(keys[key][isCapital]);
		}
	}
	else
	{
		func_key(key);
	}

	//清除键盘状态可以接受新按键
	outb(0x61, 0x7f);
	//通知PIC1可以接受新中断
	accept_new_irq(KEYBOARD_IRQ);
}

//把可能超出[0,MAX_CHARBUF]的值转换为正常下标
#define POS(pos) ((pos) == MAX_KEYBUF ? 0 : ((pos) == -1 ? MAX_KEYBUF - 1 : (pos)))
//POS(pos+1)
#define NEXT(pos) ((pos + 1) == MAX_KEYBUF ? 0 : (pos + 1))
//POS(pos-1)
#define LAST(pos) ((pos - 1) == -1 ? MAX_KEYBUF - 1 : (pos - 1))

void write_charbuf(char c)
{
	if (c == 0)
	{
		return;
	}

	//如果写满了缓冲区，丢掉后面的
	if (NEXT(pWrite) != pRead)
	{
		charbuf[pWrite] = c;
		pWrite = NEXT(pWrite);
	}
}

char read_charbuf()
{
	char c = 0;

	//如果缓冲区空了，返回0
	//tip: pWrite是还没写的那个字符
	if (pRead != pWrite)
	{
		c = charbuf[pRead];
		pRead = NEXT(pRead);
	}

	// printf("%xd",(int)c);
	//返回字符
	return c;
}

void func_key(char key)
{
}