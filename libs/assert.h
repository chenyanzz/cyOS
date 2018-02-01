/**
 * Created by chenyan on 2018/2/1.
 */

#pragma once

#include "stdio.h"

#define assert(expr)\
    if(!(expr))\
    {\
        printf("${red}assertion failed:"#expr"\n");\
    }