#include "stdarg.h"
#include "printf.h"
#include "sprintf.h"
#include "stdlib.h"
#include "asm.h"
#include "map.h"
#include "interrupt/irq.h"
#include "math.h"

static char x, y; //屏幕光标位置

static char color; //当前颜色设置

const int width = 80;
const int height = 25;

extern OutputAttrbutes attr;

//显存里字符结构体
struct Character {
    char text;
    char color;
};

struct Character *screen[80]; //[40][80]{text,color} 字符显存

char *color_key[] =
        {
                "BLACK", "DARKGRAY", "RED", "LIGHTRED", "TOMATO", "GREEN", "LIGHTGREEN", "LAWNGREEN", "BLUE", "SKYBLUE",
                "LIGHTBLUE", "ORANGE", "YELLOW", "PURPLE", "PINK", "CYAN", "LIGHTCYAN", "WHITE", "LIGHTGRAY",
                "LIGHTWHITE",

                "bgBLACK", "bgRED", "bgGREEN", "bgBLUE", "bgORANGE", "bgPURPLE", "bgCYAN", "bgWHITE",

                "normal", "defult", "default", 0}; //以0结尾

char color_value[] =
        {
                BLACK, DARKGRAY, RED, LIGHTRED, TOMATO, GREEN, LIGHTGREEN, LAWNGREEN, BLUE, SKYBLUE, LIGHTBLUE, ORANGE,
                YELLOW, PURPLE, PINK, CYAN, LIGHTCYAN, WHITE, LIGHTGRAY, LIGHTWHITE,

                bgBLACK, bgRED, bgGREEN, bgBLUE, bgORANGE, bgPURPLE, bgCYAN, bgWHITE,

                defaultColor, defaultColor, defaultColor};

extern struct OutputAttrbutes attr;

void setTerminalColor(TextColor tc, BgColor bc, bool blink) {
    color = makeColor(tc, bc) | (blink << 7);
}

void setTerminalColorByte(byte c) {
    color = c;
}

/**
 * 获取显卡寄存器里的光标位置带x,y变量里
 */
void get_reg_xy() {
    //告诉地址寄存器要接下来要使用14号寄存器
    outb(14, 0x03d4);

    //从光标位置高位寄存器读取值
    u8 cursor_pos_h = inb(0x03d5);

    //告诉地址寄存器要接下来要使用15号寄存器
    outb(15, 0x03d4);

    //从光标位置高位寄存器读取值
    u8 cursor_pos_l = inb(0x03d5);

    int cursor_pos = (cursor_pos_h << 8) + cursor_pos_l;

    y = cursor_pos / 80;
    x = cursor_pos % 80;

}

bool init_terminal() {
    //获取当前光标位置
    get_reg_xy();

    setTerminalColor(defaultTextColor, defaultBgColor, false);
    //填补screen数组
    screen[0] = (struct Character *) p_firstChar;
    for (int i = 1; i < height; i++) {
        screen[i] = screen[i - 1] + width;
    }
    return true;
}

//清屏
void cls() {
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            screen[j][i].text = ' ';
        }
    }
    x = 0;
    y = 0;
}

//控制台内容整体上移一行
void lineup() {
    //将这一行数据挪到上一行
    for (int i = 1; i < height; i++) {
        for (int j = 0; j < width; j++) {
            screen[i - 1][j] = screen[i][j];
        }
    }

    //屏幕上39行补空格
    for (int i = 0; i < width; i++) {
        screen[height - 1][i].text = ' ';
    }
}

//判断光标行溢出则换行，列溢出则上滚
void checkXY() {
    if (x >= width) {
        x = 0;
        y++;
    }
    if (y >= height) {
        y = height - 1;
        lineup();
    }
}

void setCursor(u8 nx, u8 ny) {
    x = nx;
    y = ny;
    checkXY();

    //计算光标的线性位置
    u16 cursor_pos = y * 80 + x;

    //告诉地址寄存器要接下来要使用14号寄存器
    outb(0x03d4, 14);

    //向光标位置高位寄存器写入值
    outb(0x03d5, (cursor_pos >> 8) & 0xff);

    //告诉地址寄存器要接下来要使用15号寄存器
    outb(0x03d4, 15);

    //向光标位置高位寄存器写入值
    outb(0x03d5, cursor_pos & 0xff);
}

void printChar(char ch) {

    close_int();

    switch (ch) {
        case '\n': {    //换行回车
            y++;
            x = 0;
            break;
        }
        case '\b': {    //退格
            x--;
            if (x < 0) {
                x = width - 1;
                y--;
                if (y < 0)
                    y = 0;
            }
            screen[y][x].text = ' ';
            break;
        }
        case '\t': {    //制表
            x = (x / tab_size + 1) * tab_size;
            break;
        }
        case '\f': {    //换页
            cls();
            break;
        }
        case '\r': {    //回行首
            x = 0;
            break;
        }
        case '\v': {    //垂直制表
            //打印机才有用..
            break;
        }
        default: {
            screen[y][x].text = ch;
            screen[y][x].color = color;
            x++;
        }
    }


    setCursor(x, y);

    start_int();
}

/**
 * 简单的根据所给长度打印字符串
 * @param str 字符串
 * @param len 长度
 */
void printStrSimple(char *str, int len) {
    //打印字符串
    for (int i = 0; i < len; i++) {
        printChar(str[i]);
    }
}

void printStr(char *str, int maxlen, bool bFill) {

    //字符串真实长度
    int real_len = strlen(str);

    //要打印的长度
    int print_len = maxlen == 0 ? real_len : maxlen;

    //前面填补空格
    if (bFill) {
        for (int i = real_len; i < maxlen; i++) {
            printChar(' ');
        }
    }

    printStrSimple(str, print_len);
}

void parseColor(const char *&pc) {
    //检测当前字符是否是${
    if (*pc == '$') {
        char buf[30];
        strFill(buf, 0, 30);
        char *buf2 = 0;
        pc++;
        if (*pc == '$') {
            printChar(*pc);
        } else if (*pc == '{') {
            pc++;
            int i;
            //把{}间的部分存入buf
            for (i = 0; *pc != '}'; i++) {
                buf[i] = *pc;
                pc++;
            }
            pc++;
            buf[i] = '\0';
            buf2 = 0;
            //判断buf是不是两个颜色用|分隔
            for (int j = 0; j < i; j++) {
                if (buf[j] == '|') {
                    buf[j] = '\0';
                    buf2 = buf + j + 1;
                    break;
                }
            }

            //比较buf与各种颜色
            byte tc = defaultTextColor;
            byte bc = defaultBgColor;
            if (!isEmpty(buf)) {
                tc = getValue(buf, color_key, color_value, -1, false);
            }
            if ((!isEmpty(buf2)) && buf2[0] != '}') {
                bc = getValue(buf2, color_key, color_value, -1, false);
            }
            color = makeColor(tc, bc);
        }
    }
}

// int cprintf(const char *format, ...)
// {
// 	va_list vl;
// 	va_start(vl, format);

// 	char buf[1000];
// 	int r = sprintf(buf, format, vl);
// 	printStr(buf);
// 	return r;
// }

int printf(const char *format, ...) {
    //准备可变参数
    va_list vl;
    va_start(vl, format);

    int params = 0;

    static char buf[100] = {0};

    const char *pc = format;
    while (*pc != 0) {
        //判断是否是设置颜色的${。。。}
        parseColor(pc);

        //如果不是%则正常的输出
        if (*pc != '%') {
            printChar(*pc);
            pc++;
            continue;
        }

        //判断%后的字符是什么
        pc++;

        //%[sign][width][.dp]  捕获%后面属性
        parseAttr(pc, vl);
        params++;
        switch (*pc) {
            case 'c':
                buf[0] = va_arg(vl, char);
                buf[1] = 0;
                break;
            case 's':
                printStr(va_arg(vl, char *), attr.width, true);
                pc++;
                continue;
            case 'u':
                sprintUInt(va_arg(vl, unsigned int), buf);
                break;
            case 'd':
                sprintInt(va_arg(vl, int), buf);
                break;
            case 'x':
                buf[0] = '0';
                buf[1] = 'x';
                sprintHex(va_arg(vl, unsigned int), buf + 2, false);
                break;
            case 'X':
                buf[0] = '0';
                buf[1] = 'x';
                sprintHex(va_arg(vl, unsigned int), buf + 2, true);
                break;
            case 'f':
                sprintDouble(va_arg(vl, double), attr.dp, buf);
                break;
            case 'l':
                pc++;
                switch (*pc) {
                    case 'u':
                        sprintUInt(va_arg(vl, unsigned long), buf);
                        break;
                    case 'd':
                        sprintInt(va_arg(vl, long), buf);
                        break;
                    case 'x':
                        sprintHex(va_arg(vl, unsigned long), buf, false);
                        break;
                    case 'X':
                        sprintHex(va_arg(vl, unsigned long), buf, true);
                        break;
                    case 'f':
                        sprintDouble(va_arg(vl, double), attr.dp, buf);
                        break;
                    case 'l':
                        pc++;
                        switch (*pc) {
                            case 'u':
                                sprintUInt(va_arg(vl, unsigned long long), buf);
                                break;
                            case 'd':
                                sprintInt(va_arg(vl, long long), buf);
                                break;
                            case 'x':
                                buf[0] = '0';
                                buf[1] = 'x';
                                sprintHex(va_arg(vl, unsigned long long), buf + 2, false);
                                break;
                            case 'X':
                                buf[0] = '0';
                                buf[1] = 'x';
                                sprintHex(va_arg(vl, unsigned int), buf + 2, true);
                                break;
                        }
                        break;
                }
                break;
            case 'h':
                pc++;
                switch (*pc) {
                    case 'u':
                        sprintUInt(va_arg(vl, unsigned short), buf);
                        break;
                    case 'd':
                        sprintInt(va_arg(vl, short), buf);
                        break;
                    case 'x':
                        buf[0] = '0';
                        buf[1] = 'x';
                        sprintHex(va_arg(vl, unsigned short), buf + 2, false);
                        break;
                    case 'X':
                        buf[0] = '0';
                        buf[1] = 'x';
                        sprintHex(va_arg(vl, unsigned short), buf + 2, true);
                        break;
                    case 'h':
                        pc++;
                        switch (*pc) {
                            case 'u':
                                sprintUInt(va_arg(vl, unsigned char), buf);
                                break;
                            case 'd':
                                sprintInt(va_arg(vl, char), buf);
                                break;
                            case 'x':
                                buf[0] = '0';
                                buf[1] = 'x';
                                sprintHex(va_arg(vl, unsigned char), buf + 2, false);
                                break;
                            case 'X':
                                buf[0] = '0';
                                buf[1] = 'x';
                                sprintHex(va_arg(vl, unsigned char), buf + 2, true);
                                break;
                        }
                        break;
                }
                break;
            default:
                printChar(*pc);
                params--;
        }
        printStr(buf, attr.width, true);
        buf[0] = 0;

        pc++;
    }
    va_end(vl);
    return params;
}

void setXY(const int newx, const int newy) {
    close_int();

    x = newx;
    y = newy;

    start_int();
}