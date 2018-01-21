;导出这些函数，注意要带‘_’
global _deal_int_13
global _deal_int_0
global _deal_irq_0
global _deal_irq_1

[section .text]

;压栈环境
%macro save_env 0
	cli
	pushf
	push ds
	push es
	push fs
	push gs
	push ss
	pushad
	sti
%endmacro

;弹栈环境
%macro restore_env 0
	cli
	popad
	pop ss
	pop gs
	pop fs
	pop es
	pop ds
	popf
	sti
%endmacro

_deal_int_13:
	jmp _general_protection_fault_handler

_deal_int_0:
	call _devide_zero_handler
	iret

_deal_irq_0:
	save_env
	call _timer_tick
	restore_env
	iret

_deal_irq_1:
	save_env
	call _key_state_changed
	restore_env
	iret