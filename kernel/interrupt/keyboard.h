#pragma once

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
 * 向字符缓冲区中添加一个字符
 */
void write_charbuf(char c);

/**
 * 从字符缓冲区中读取一个字符
 */
char read_charbuf();

/**
 * 响应功能键
 */
void func_key(char key);


