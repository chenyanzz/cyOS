/**
 * Created by chenyan on 2018/2/1.
 */

#include "bitmap.h"
#include "stdlib.h"
#include "asm.h"
#include "math.h"

void bp_init(BITMAP& bp,bool val) {
    setBits(bp,0,bp.count_bits,val);
}

inline void setBit(BITMAP bp, BIT b, bool val) {
    if (val == 1) {
        setb(bp.bytes[b / 8], b);
    } else {
        clrb(bp.bytes[b / 8], b);
    }
}

void setBits(BITMAP bp, BIT from, int bits, bool val) {
    BIT to = from+bits-1;//要修改的最后一位

    BIT b;//现在在修改的位


    //把from对其到整字节开头，设置from到整字节的部分
    for (b = from; b % 8 != 0; b++) {
        setBit(bp, b, val);
    }

    int bytes = (to - b) / 8;//有几个整字节
    memset(&bp.bytes[b / 8], val == 0 ? 0x00 : 0xFF, bytes);//设置整字节部分

    //设置整字节到to的部分
    for (int i = 0; i < to % 8; i++) {
        setBit(bp, to - i, val);
    }
}

inline bool getBit(BITMAP bp, BIT b) {
    return bit(bp.bytes[b / 8], b % 8);
}

/**
 * 寻找第一个不是 8位全val 的字节
 * @return 那一字节下标
 */
int getFirstByteIsntVal(BITMAP &bp, bool val) {
    int byte;
    //把全不是val值的字节跳过
    for (byte = 0; bp.bytes[byte] = (val == 0 ? 0xFF : 0x00); byte++) {
        if (byte >= ceil(bp.count_bits / 8.0f))break;
    }
    return byte;
}

BIT findBit(BITMAP bp, bool val = 1) {

    //第一个8位不全是！val的字节编号
    int nByte;

    //把全不是val值的字节跳过
    for (nByte = 0; bp.bytes[nByte] = (val == 0 ? 0xFF : 0x00); nByte++) {
        if (nByte >= ceil(bp.count_bits / 8.0f))break;
    }

    //检索第一个不全是!val的字节
    int b;
    for (b = 0; b < bp.count_bits - nByte * 8; b++) {
        if (bit(bp.bytes[nByte], b) == val) {
            return nByte * 8 + b;
        }
    }

    //没找到返回-1
    return -1;
}

BIT findBits(BITMAP bp, int bits, bool val = 1) {
    int bytes = bits / 8 - 1;//至少包含几个完整字节

    int count = 0;
    for (int i = 0; i < bp.count_bits / 8; i++) {
        //遍历每个字节
        if (bp.bytes[i] == (val == 1 ? 0xFF : 0)) {
            //如果这一字节符合要求
            count++;
        } else {
            count = 0;
        }

        if (count == i) {

            BIT from = i * 8;
            //检索前一字节
            for (int j = 0; j < 8; j++) {
                if (getBit(bp, from - 1) == val) {
                    from--;
                } else {
                    break;
                }
            }

            BIT to = (i + bytes) * 8 - 1;
            //检索后一字节
            for (int j = 0; j < bp.count_bits%8; j++) {
                if (getBit(bp, to + 1) == val) {
                    to++;
                } else {
                    break;
                }
            }

            if (to - from >= bits) {
                setBits(bp, from, from + bits - 1);
                return from;
            }else{
                count=0;
            }
        }
    }
    return -1;
}