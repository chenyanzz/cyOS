#ifndef KEYBOAR_H
#define KEYBOAR_H

#define MAX_KEYBUF 200

/*
 *初始化键盘
 */
bool init_keyboard();

/**
 * 处理键盘中断
 */
extern "C" void key_state_changed();

/**
 * 开启键盘中断
 */
void start_keyboard_int();

/**
 * 向字符缓冲区中添加一个字符
 */
void set_charbuf(char c);

/**
 * 从字符缓冲区中读取一个字符
 */
char get_charbuf();

#endif //KEYBOAR_H
