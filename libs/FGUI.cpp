#include "FGUI.h"
#include "printf.h"
#include "stdlib.h"
#include "stdarg.h"

char *color[] =
    {
        "BLACK", "DARKGRAY", "RED", "LIGHTRED", "TOMATO", "GREEN", "LIGHTGREEN", "LAWNGREEN", "BLUE", "SKYBLUE", "LIGHTBLUE", "ORANGE", "YELLOW", "PURPLE", "PINK", "CYAN", "LIGHTCYAN", "WHITE", "LIGHTGRAY", "LIGHTWHITE", //TextColor有可能会打错
        "bgBLACK", "bgRED", "bgGREEN", "bgBLUE", "bgORANGE", "bgPURPLE", "bgCYAN", "bgWHITE"                                                                                                                                 //BgColor有可能会打错，P(打错BgColor) < P(打错TextColor)
};

char colorB[] =
    {
        BLACK, DARKGRAY, RED, LIGHTRED, TOMATO, GREEN, LIGHTGREEN, LAWNGREEN, BLUE, SKYBLUE, LIGHTBLUE, ORANGE, YELLOW, PURPLE, PINK, CYAN, LIGHTCYAN, WHITE, LIGHTGRAY, LIGHTWHITE, //TextColor有可能会打错
        bgBLACK, bgRED, bgGREEN, bgBLUE, bgORANGE, bgPURPLE, bgCYAN, bgWHITE                                                                                                         //BgColor有可能会打错，P(打错BgColor) < P(打错TextColor)
};

void showBootScreen(void)
{
    char *str = "made by ${ORANGE}us";
    cls();
    setXY((WIDTH - strlen(str)) / 2, HEIGHT / 2);
    //printf(str);
    printColorStr(str);
}

void printColorStr(char *colorStr, ...)
{
    //准备可变参数
	va_list vl;
	va_start(vl, colorStr);
    char buf[10];
    char *str_start;//送给printf的字符串开始指针

    //对于每个字符循环
    for (; *colorStr != '\0'; colorStr++)
    {

        //检测当前字符是否是${
        if (*colorStr == '$')
        {
            colorStr++;
            if (*colorStr == '$')
            {
                continue;
            }
            if (*colorStr == '{')
            {
                colorStr++;
                int i;
                for(i = 0; *colorStr != '}'; i++)
                {
                    buf[i] = *colorStr;
                    colorStr++;
                }
                buf[i] = '\0';
            }
        }
    }
}