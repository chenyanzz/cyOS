/**
 * Created by chenyan on 2018/2/1.
 */

#pragma once

#include "asm.h"

typedef int BIT;///<位的编号

typedef struct {
    byte* bytes;
    int count_bits;
} BITMAP;

/**
 * 初始化一个位图
 * @param val 初始值
 */
void bp_init(BITMAP& bp,bool val);

inline void setBit(BITMAP bp, BIT b, bool val = 1);

/**
 * 设置[from,from+bits)位为val
 */
void setBits(BITMAP bp, BIT from,int bits, bool val = 1);

inline bool getBit(BITMAP bp, BIT b);

/**
 * 寻找值为val的一位
 * @return 那一位的编号
 * @retval -1 未找到
 */
BIT findBit(BITMAP bp, bool val = 1);

/**
 * 寻找值为val的连续几位
 * @param bits 连续的位数
 * @return 头一位的编号
 * @retval -1 未找到
 */
BIT findBits(BITMAP bp,int bits, bool val = 1);