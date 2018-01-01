#ifndef KEY_MAP_H
#define KEY_MAP_H

#include "types.h"

//定义按键扫描码的名称
#define KEY_ESC 0x01		 // ESC
#define KEY_1 0x02			 // 1
#define KEY_2 0x03			 // 2
#define KEY_3 0x04			 // 3
#define KEY_4 0x05			 // 4
#define KEY_5 0x06			 // 5
#define KEY_6 0x07			 // 6
#define KEY_7 0x08			 // 7
#define KEY_8 0x09			 // 8
#define KEY_9 0x0A			 // 9
#define KEY_0 0x0B			 // 0
#define KEY_DASH 0x0C		 // -
#define KEY_EQUAL 0x0D		 // =
#define KEY_BACKSPACE 0x0E   // BACKSPACE
#define KEY_TAB 0x0F		 // TAB
#define KEY_Q 0x10			 // Q
#define KEY_W 0x11			 // W
#define KEY_E 0x12			 // E
#define KEY_R 0x13			 // R
#define KEY_T 0x14			 // T
#define KEY_Y 0x15			 // Y
#define KEY_U 0x16			 // U
#define KEY_I 0x17			 // I
#define KEY_O 0x18			 // O
#define KEY_P 0x19			 // P
#define KEY_LBRACKET 0x1A	// [
#define KEY_RBRACKET 0x1B	// ]
#define KEY_ENTER 0x1C		 // ENTER
#define KEY_CTRL 0x1D		 // CTRL
#define KEY_A 0x1E			 // A
#define KEY_S 0x1F			 // S
#define KEY_D 0x20			 // D
#define KEY_F 0x21			 // F
#define KEY_G 0x22			 // G
#define KEY_H 0x23			 // H
#define KEY_J 0x24			 // J
#define KEY_K 0x25			 // K
#define KEY_L 0x26			 // L
#define KEY_SEMICOLON 0x27   // ;
#define KEY_RQUOTE 0x28		 // '
#define KEY_LQUOTE 0x29		 // `
#define KEY_LEFT_SHIFT 0x2A  // LEFT SHIFT
#define KEY_BACKSLASH 0x2B   // '\'
#define KEY_Z 0x2C			 // Z
#define KEY_X 0x2D			 // X
#define KEY_C 0x2E			 // C
#define KEY_V 0x2F			 // V
#define KEY_B 0x30			 // B
#define KEY_N 0x31			 // N
#define KEY_M 0x32			 // M
#define KEY_COMMA 0x33		 // ,
#define KEY_PERIOD 0x34		 // .
#define KEY_SLASH 0x35		 // /
#define KEY_RIGHT_SHIFT 0x36 // RIGHT SHIFT
#define KEY_PRTSC 0x37		 // PRINT SCREEN
#define KEY_ALT 0x38		 // ALT
#define KEY_SPACE 0x39		 // SPACE
#define KEY_CAPS_LOCK 0x3A   // CAPS LOCK
#define KEY_F1 0x3B			 // F1
#define KEY_F2 0x3C			 // F2
#define KEY_F3 0x3D			 // F3
#define KEY_F4 0x3E			 // F4
#define KEY_F5 0x3F			 // F5
#define KEY_F6 0x40			 // F6
#define KEY_F7 0x41			 // F7
#define KEY_F8 0x42			 // F8
#define KEY_F9 0x43			 // F9
#define KEY_F10 0x44		 // F10
#define KEY_NUM_LOCK 0x45	// NUM LOCK
#define KEY_SCROLL_LOCK 0x46 // SCROLL LOCK
#define KEY_HOME 0x47		 // HOME
#define KEY_UP 0x48			 // UP
#define KEY_PAGE_UP 0x49	 // PAGE UP
#define KEY_SUB 0x4A		 // SUB
#define KEY_LEFT 0x4B		 // LEFT
#define KEY_CENTER 0x4C		 // CENTER
#define KEY_RIGHT 0x4D		 // RIGHT
#define KEY_ADD 0x4E		 // ADD
#define KEY_END 0x4F		 // END
#define KEY_DOWN 0x50		 // DOWN
#define KEY_PAGE_DOWN 0x51   // PAGE DOWN
#define KEY_INSERT 0x52		 // INSERT
#define KEY_DEL 0x53		 // DEL

u8 keys[][2] =
	{
		{0x0, 0x0},   //对齐到1开头
		{0x0, 0x0},   // ESC
		{'1', '!'},   // 1
		{'2', '@'},   // 2
		{'3', '#'},   // 3
		{'4', '$'},   // 4
		{'5', '%'},   // 5
		{'6', '^'},   // 6
		{'7', '&'},   // 7
		{'8', '*'},   // 8
		{'9', '('},   // 9
		{'0', ')'},   // 0
		{'-', '_'},   // -
		{'=', '+'},   // =
		{'\b', '\b'}, // BACKSPACE
		{'\t', '\t'}, // TAB
		{'q', 'Q'},   // Q
		{'w', 'W'},   // W
		{'e', 'E'},   // E
		{'r', 'R'},   // R
		{'t', 'T'},   // T
		{'y', 'Y'},   // Y
		{'u', 'U'},   // U
		{'i', 'I'},   // I
		{'o', 'O'},   // O
		{'p', 'P'},   // P
		{'[', '{'},   // [
		{']', '}'},   // ]
		{'\n', '\n'}, // ENTER
		{0x0, 0x0},   // CTRL
		{'a', 'A'},   // A
		{'s', 'S'},   // S
		{'d', 'D'},   // D
		{'f', 'F'},   // F
		{'g', 'G'},   // G
		{'h', 'H'},   // H
		{'j', 'J'},   // J
		{'k', 'K'},   // K
		{'l', 'L'},   // L
		{';', ':'},   // ;
		{'\'', '"'},  // '
		{'`', '~'},   // `
		{0x0, 0x0},   // LEFTSHIFT
		{'\\', '|'},  // '\'
		{'a', 'Z'},   // Z
		{'x', 'X'},   // X
		{'c', 'C'},   // C
		{'v', 'V'},   // V
		{'b', 'B'},   // B
		{'n', 'N'},   // N
		{'m', 'M'},   // M
		{',', '<'},   // ,
		{'.', '>'},   // .
		{'/', '?'},   // /
		{0x0, 0x0},   // RIGHTSHIFT
		{0x0, 0x0},   // PRINTSCREEN
		{0x0, 0x0},   // ALT
		{' ', ' '},   // SPACE
		{0x0, 0x0},   // CAPSLOCK
		{0x0, 0x0},   // F1
		{0x0, 0x0},   // F2
		{0x0, 0x0},   // F3
		{0x0, 0x0},   // F4
		{0x0, 0x0},   // F5
		{0x0, 0x0},   // F6
		{0x0, 0x0},   // F7
		{0x0, 0x0},   // F8
		{0x0, 0x0},   // F9
		{0x0, 0x0},   // F10
		{0x0, 0x0},   // NUMLOCK
		{0x0, 0x0},   // SCROLLLOCK
		{0x0, 0x0},   // HOME
		{0x0, 0x0},   // UP
		{0x0, 0x0},   // PAGEUP
		{0x0, 0x0},   // SUB
		{0x0, 0x0},   // LEFT
		{0x0, 0x0},   // CENTER
		{0x0, 0x0},   // RIGHT
		{0x0, 0x0},   // ADD
		{0x0, 0x0},   // END
		{0x0, 0x0},   // DOWN
		{0x0, 0x0},   // PAGEDOWN
		{0x0, 0x0},   // INSERT
		{0x0, 0x0}	// DEL
};

#endif //KEY_MAP_H
