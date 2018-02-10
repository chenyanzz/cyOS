/**
 * Created by chenyan on 2018/2/9.
 */

#pragma once

#include "types.h"

struct END_TAG{
    u16 type;///< 0
    u16 flags;
    u16 size;///< 8
};

///GRUB要求的头
struct Multiboot2_Header {
    u32 magic;///< 魔数 @see MULTIBOOT_MAGIC
    u32 architecture;///< 指令集标志 @see MULTIBOOT_ARCH
    u32 header_length;///< 头长度
    u32 checksum;///< 校验值

    /*tags*/
    END_TAG end_tag;
};

#define MULTIBOOT_MAGIC (0xE85250D6)///< 必须是这个数
#define MULTIBOOT_ARCH (0)///< 0表示 32位i386

extern Multiboot2_Header multiboot2_header __attribute__((section(".grub_sig")));