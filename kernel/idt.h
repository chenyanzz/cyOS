#ifndef IDT_H
#define IDT_H

#include "types.h"

struct idt_descriptor
{
	u16 offset;
	u16 segment_selector;
};

#endif //IDT_H
