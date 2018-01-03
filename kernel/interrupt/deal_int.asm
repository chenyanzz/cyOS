global _deal_int_13
global _deal_int_0
global _deal_irq_0
global _deal_irq_1

[section .text]

%macro push_reg 0
	cli
	pushad
	push ds
	push es
	push ss
	push fs
	push gs
	pushf
	sti
%endmacro

%macro pop_reg 0
	cli
	popf
	pop gs
	pop fs
	pop ss
	pop es
	pop ds
	popad
	sti
%endmacro

_deal_int_13:
	jmp _general_protection_fault_handler

_deal_int_0:
	call _devide_zero_handler
	iret

_deal_irq_0:
	push_reg
	call _timer_tick
	pop_reg
	iret

_deal_irq_1:
	push_reg
	call _key_state_changed
	pop_reg
	iret