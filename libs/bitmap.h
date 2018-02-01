/**
 * Created by chenyan on 2018/2/1.
 */

#pragma once

#include "asm.h"

typedef int BIT;///<位的编号

typedef byte* BITMAP;


#define create_bitmap(bits,name) byte name[bits/8]

inline void setBit(BITMAP bp,BIT b,bool val=1);

void setBits(BITMAP bp,BIT from,BIT to,bool val=1);

void setBits(BITMAP bp,BIT from,int lenb,bool val=1);

inline bool getBit(BITMAP bp,BIT b);

/**
 * 寻找值为val的一位
 * @param bits bp总共多少位
 * @return 那一位的编号
 * @retval -1 未找到
 */
BIT findBit(BITMAP bp,int bits, bool val=1);