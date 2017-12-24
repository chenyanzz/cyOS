#ifndef FGUI_H
#define FGUI_H

//Fake Graphical User Interface
#include "stdio.h"

/**
 * 显示启动界面
 */
void showBootScreen(void);

/**
 * 显示带颜色字符串
 * @param colorStr ${前景色,背景色}后的字符按照设置的颜色输出
 * 	依然可以使用printf的格式字符串
 */
void printColorStr(char* colorStr, ...);

#define WIDTH 80

#define HEIGHT 25

#endif //FGUI_H