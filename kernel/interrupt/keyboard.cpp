#include "keyboard.h"
#include "key_map.h"
#include "asm.h"
#include "stdio.h"

struct KeyState
{
	bool isShift : 1;
	bool isCtrl : 1;
	bool isAlt : 1;
	bool isCapsLk : 1;
	bool isNumLk : 1;
	bool isSclLk : 1;
} kstat;

bool init_keyboard()
{
	start_keyboard_int();
	return true;
}

extern "C" void key_state_changed()
{
	byte scan_code = inb(0x60);
	// printf("%xd ", (int)scan_code);

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

	if(isDown)
	{
		printChar(keys[key][kstat.isShift]);
	}
	//清除键盘状态可以接受新按键
	outb(0x61, 0x7f);
	//通知PIC1可以接受新中断
	outb(0x20, 0x20);
}

void start_keyboard_int()
{
	//打开IRQ1的键盘中断
	outb(0x21, inb(0x21) & 0xfd);
}