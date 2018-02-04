//
// Created by chenyan on 2018/1/18.
//

#include <kernel/interrupt/irq.h>
#include "malloc.h"
#include "page.h"
#include "stdlib.h"

void *malloc(int size) {

    static u32 page = (u32)allocPage();
    static u32 offset = 0;
    u32 ret;

    close_int();
    offset+=size;
    ret = page+offset;

    start_int();

    return (void*)ret;
}
