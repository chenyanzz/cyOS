/**
 * Created by chenyan on 2018/2/1.
 */

#include "bitmap.h"
#include "stdlib.h"
#include "asm.h"
#include "math.h"

inline void setBit(BITMAP bp, BIT b, bool val) {
    if (val == 1) {
        setb(bp[b / 8], b);
    } else {
        clrb(bp[b / 8], b);
    }
}

void setBits(BITMAP bp, BIT from, BIT to, bool val) {
    BIT b;//现在在修改的位

    //把from对其到整字节开头，设置from到整字节的部分
    for (b = from; b % 8 != 0; b++) {
        setBit(bp, b, val);
    }

    int bytes = (to - b) / 8;//有几个整字节
    memset(&bp[b / 8], val == 0 ? 0x00 : 0xFF, bytes);//设置整字节部分

    //设置整字节到to的部分
    for (int i = 0; i < to % 8; i++) {
        setBit(bp, to - i, val);
    }
}

inline bool getBit(BITMAP bp, BIT b) {
    return bit(bp[b / 8], b % 8);
}

BIT findBit(BITMAP bp, int bits, bool val = 1) {
    int byte;

    //把全不是val值的字节跳过
    for (byte = 0; bp[byte] = (val == 0 ? 0xFF : 0x00); byte++) {
        if (byte >= ceil(bits / 8.0f))break;
    }

    //检索第一个不全是!val的字节
    int b;
    for (b = 0; b < bits - byte * 8; b++) {
        if (bit(bp[byte], b) == val) {
            return byte * 8 + b;
        }
    }

    //没找到返回-1
    return -1;
}