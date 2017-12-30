#ifndef KEYBOAR_H
#define KEYBOAR_H

/*
 *初始化键盘
 */
bool init_keyboard();

/**
 * 处理键盘中断
 */
extern "C"
void key_state_changed();

/**
 * 开启键盘中断
 */ 
void start_keyboard_int();

#endif //KEYBOAR_H
