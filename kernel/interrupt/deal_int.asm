global _deal_int_13
global _deal_int_0
global _deal_irq_0
global _deal_irq_1

[section .text]
_deal_int_13:
	call _general_protection_fault_handler
	iret

_deal_int_0:
	call _devide_zero_handler
	iret

_deal_irq_0:
	cli
	call _timer_tick
	sti
	iret

_deal_irq_1:
	cli
	call _key_state_changed
	sti
	iret