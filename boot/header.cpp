#include "grub.h"

/**
 * Created by chenyan on 2018/2/9.
 * 这个文件放在.text代码段开头
 */

Multiboot2_Header multiboot2_header = {
        MULTIBOOT_MAGIC, MULTIBOOT_ARCH,sizeof(Multiboot2_Header),
        0 - MULTIBOOT_MAGIC - MULTIBOOT_ARCH - sizeof(Multiboot2_Header),
        {0,0,8}
};