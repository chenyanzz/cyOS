ENTRY(_kernel_main)

SECTIONS
{
    . = 1M;

    .boot :
    {
        *(.grub_sig)
    }
    .text :
    {
        build/kernel/kernel/main.cpp.o (.text)
        *(.text)
    }
    .data :
    {
        *(.data)
    }
    .bss :
    {
        *(.bss)
    }
}