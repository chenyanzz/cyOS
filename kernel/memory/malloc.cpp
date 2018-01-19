//
// Created by chenyan on 2018/1/18.
//

#include "malloc.h"
#include "page.h"
#include "stdlib.h"

static void* current_page;


// bool init_memory()
// {
//     initNewPage();
// }

// void initNewPage()
// {
//     void *p = allocPage();
//     memset(p,128,0);
//     current_page  = p;
// }