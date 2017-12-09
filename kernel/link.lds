ENTRY(_start)
SECTIONS
{
.text 0:{build/kernel/kernel/main.cpp.o (.text) *(.text)}
}