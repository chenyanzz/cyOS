;prepare for & jump to protected mode
;=========================================================
KERNELSEG equ 1000h
DATASEG equ 9000h
SETUPSEG equ 9020h
;KERNEL_SIZE_512 --later define on cmd -dxxx=xx

save_bios_data:
;save bios data to DATASEG:
;0000   cursor pos
;   [0]:col
;   [1]:row
;0002   extended memory size
;0004   VGA display mode
;0010   harddisk settings[16byte*2disks]
;---------------------------------------------------------


;set data segment
mov ax,DATASEG
mov ds,ax

;cursor pos
mov ah,3
mov bh,0
int 10h
mov [ds:0],dx

;extended memory size
mov ah,88h
int 15h
mov [ds:2],ax

;VGA display mode
mov ah,12h
mov bl,10h
int 10h
mov bx,[ds:4]
mov cx,[ds:6]

;harddisk settings
cld;set si,di -> ++
;disk0
mov ax,0
mov ds,ax
lds si,[ds:4*41h]
mov es,ax
mov di,0010h
mov cx,10h
rep movsb
;disk1
mov ax,0
mov ds,ax
lds si,[ds:4*46h]
mov es,ax
mov di,0020h
mov cx,10h
rep movsb
;clear disk1 if =null
mov ah,15h
mov dl,81h
int 13h
jc clear_disk1
cmp ah,3
je copy_data_end

clear_disk1:
    mov ax,ds
    mov es,ax
    mov di,0020h
    mov cx,10h
    mov ax,0
    rep stosb
copy_data_end:

;move binary image from KERNELSEG to 0
;---------------------------------------------------------
move_sys:
;init for copy
mov ax,KERNELSEG
mov ds,ax
mov si,0
mov bx,0
mov es,bx
mov di,0

mov ax,1;num of current kb
do_move:
cmp ax,KERNEL_SIZE_512*2
jg move_sys_end;if current >limit break
mov cx,1024;move 1k
rep movsb
inc ax;curr kb++
;add si,1024
;add di,1024

jmp do_move;loop

move_sys_end:

;set gdt
;---------------------------------------------------------
set_gdt:
    mov ax,SETUPSEG     ;0x9028d
    mov ds,ax
    lgdt [gdt_decriptor]
    jmp set_gdt_end     ;0x90297

gdt_decriptor:      ;48bits=6bytes
    dw 2048    ;size 256*(8 per)
    dd 0x90200+gdt_data;gdt addr

gdt_data:
    ;null for first
    dw 0,0,0,0
    
    ;kernel code segment
    dw 0xFFFF;limit:8MB
    dw 0;base addr
    db 0
    db 10011010b;attr
    db 11000000b
    db 0;base addr
    
    ;kernel data segment
    dw 0xFFFF;limit:8MB
    dw 0;base addr
    db 0;base addr
    db 10010010b;attr
    db 11000000b
    db 0;base addr
set_gdt_end:


;set idt
;---------------------------------------------------------
set_idt:
    lidt [idt_decriptor]
    jmp set_idt_end
idt_decriptor:
    dw 0
    dw 0,0
idt_data:

set_idt_end:

;program for the interrupt controller
;---------------------------------------------------------
init_8259A:;such a messy procedure...

    ;this subprocess copys from linux-0.1

    mov	al,0x11		;initialization sequence
	out	0x20,al		;send it to 8259A-1
	call	delay
	out	0xA0,al		;and to 8259A-2
	call	delay
	mov	al,0x20		;start of hardware int's (0x20)
	out	0x21,al
	call	delay
	mov	al,0x28		;start of hardware int's 2 (0x28)
	out	0xA1,al
	call	delay
	mov	al,0x04		;8259-1 is master
	out	0x21,al
	call	delay
	mov	al,0x02		;8259-2 is slave
	out	0xA1,al
	call	delay
	mov	al,0x01		;8086 mode for both
	out	0x21,al
	call	delay
	out	0xA1,al
	call	delay
	mov	al,0xFF		;mask off all interrupts for now
	out	0xA1,al
	call	delay
	mov	al,0xFB		;mask all irq's but irq2 which
	out	0x21,al		;is cascaded
    jmp init_8259A_end
init_8259A_end:

;jump to protected mode
;and jump to kernel
;---------------------------------------------------------
jump_to_protected_mode:
    mov eax,cr0;9020:0156
    or eax,1
    mov cr0,eax
    mov ax,10h;10 00 0
    mov ss,ax
    mov ds,ax
    mov es,ax
    mov fs,ax
    mov gs,ax
    cli

    ; mov eax,0xb8000
    ; mov esi,eax
    ; mov cx,100
    ; disp:
    ; mov byte [ds:esi],0
    ; add esi,2
    ; loop disp
    ; jmp $

    jmp 8:0;segment selector : 1 00 0;
    ;vb 8:0

;delay for I/O
delay:
	dw 0x00eb ;jmp $+2
	ret