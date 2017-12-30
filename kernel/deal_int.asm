global _deal_int_13

[section .text]
_deal_int_13:
	call _general_protection_fault_handler
	iret