/**
 * Created by chenyan on 2018/2/1.
 */
#pragma once

#include "stdio.h"


/**
 * 初始化用的宏，用来打印是否成功
 */
#ifndef OS_DEBUG
#define init(name)
if(!init_##name())\
    {\
        printf("init"#name"failed!!");\
        while(true);\
    }
#else
#define init(name)\
    printf("${normal}init " #name "\t");\
    if (init_##name())\
    {\
        printf("${blue}[succeed]\n");\
    }\
    else\
    {\
        printf("${red}[failed!]\n");\
    }
#endif