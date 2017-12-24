#include "FGUI.h"
#include "printf.h"
#include "stdlib.h"

char * color[] = 
{
    "BLACK", "DARKGRAY", "RED", "LIGHTRED", "TOMATO" , "GREEN", "LIGHTGREEN", "LAWNGREEN", "BLUE", "SKYBLUE", "LIGHTBLUE", "ORANGE", "YELLOW", "PURPLE", "PINK" ,"CYAN", "LIGHTCYAN", "WHITE", "LIGHTGRAY", "LIGHTWHITE", //TextColor有可能会打错
    "bgBLACK", "bgRED", "bgGREEN", "bgBLUE", "bgORANGE", "bgPURPLE", "bgCYAN", "bgWHITE"//BgColor有可能会打错，P(打错BgColor) < P(打错TextColor)
};

char colorB [] = 
{
    BLACK, DARKGRAY, RED, LIGHTRED, TOMATO , GREEN, LIGHTGREEN, LAWNGREEN, BLUE, SKYBLUE, LIGHTBLUE, ORANGE, YELLOW, PURPLE, PINK ,CYAN, LIGHTCYAN, WHITE, LIGHTGRAY, LIGHTWHITE, //TextColor有可能会打错
    bgBLACK, bgRED, bgGREEN, bgBLUE, bgORANGE, bgPURPLE, bgCYAN, bgWHITE//BgColor有可能会打错，P(打错BgColor) < P(打错TextColor)
};


void bootScreen(void)
{
    char * str = "made by ${ORANGE}us";
    cls();
    setXY((WIDTH - strlen(str))/2, HEIGHT/2);
    //printf(str);
    printColorStr(str);
}

void printColorStr(char* format)
{
    int i = 0;
    int j = 0;
    for (i = 0; format[i] != '\0'; i++)
    {
        if (format[i] == '$' && format[++i] == '{')
        {
            continue;
        }
        setTerminalColor(WHITE, bgBLACK, false);//默认
        printChar(format[i]);
    }
}