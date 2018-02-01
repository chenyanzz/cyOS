;导出这些函数，注意要带‘_’
global _deal_int_13
global _deal_int_0
global _deal_unexpected_int

global _deal_irq_0
global _deal_irq_1
global _deal_irq_8

[section .text]

;压栈环境
%macro save_env 0
	cli
	pushf
	pushad
	sti
%endmacro

;弹栈环境
%macro restore_env 0
	cli
	popad
	popf
	sti
%endmacro

_deal_int_13:
	jmp _general_protection_fault_handler

_deal_int_0:
	jmp _devide_zero_handler

_deal_unexpected_int:
    jmp _unexpected_int_handler

_deal_irq_0:
    save_env
    call _PIT_timer_tick
	restore_env
    iret

_deal_irq_1:
	save_env
	call _key_state_changed
	restore_env
	iret

_deal_irq_8:
    save_env
    call _RTC_timer_tick
    restore_env
    iret

