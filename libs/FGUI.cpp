#include "FGUI.h"

void showBootScreen(void)
{
    char *str = "made by ${ORANGE}us";
    cls();
    setXY((WIDTH - colorStrlen(str)) / 2, HEIGHT / 2);
    //printf(str);
    printf(str);
}