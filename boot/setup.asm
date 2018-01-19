;prepare for & jump to protected mode
;=========================================================
KERNELSEG equ 1000h
DATASEG equ 9000h
SETUPSEG equ 9020h
;KERNEL_SIZE_512 ->later define on cmd -dxxx=xx

KERNEL_TOTAL_SOZE equ (1024*1024*10)

;enable a20 for memory beyond 1MB
;---------------------------------------------------------
enable_a20:
    ;there are three methods
    
    ;;BIOS int
    mov ax,0x2401
    int 15h
    call testa20
    ;;port on board
    in al,0x92
    or al,2
    and al,0b11111110
    out 0x92,al
    call testa20
    ;;keyboard controller
    call    empty_8042
    mov     al,0xd1
    out     0x64,al
    call    empty_8042
    mov     al,0xdf
    out     0x60,al
    call    empty_8042
    call testa20


    ;get current cursor position
    mov bx,0;page
    mov ax,0300h
    int 10h

    ;write error str
    mov ax,cs
    mov es,ax;str segment
    mov ax,a20error_str
    mov bp,ax;str addr
    mov cx,a20error_str_end-a20error_str;str len
    mov bx,7;color
    mov ax,1301h
    int 10h

    ;die
    jmp $
empty_8042:
    dw    00ebh,00ebh            
    in    al,64h            
    test  al,2
    jnz   empty_8042
    ret

testa20:
    ;0xffff:0x10 = 0x100000
    mov ax,0
    mov es,ax
    mov ax,0xFFFF
    mov gs,ax
    mov ax,111
    mov bx,233
    mov [es:0x1],ax
    mov [gs:0x11],bx
    cmp [gs:0x11],ax
    jne enable_a20_end
    ret

enable_a20_end:

save_bios_data:
;save bios data to DATASEG:
;0000   cursor pos
;   [0]:col
;   [1]:row
;0002   extended memory size
;0004   VGA display mode
;0010   harddisk settings[16byte*2disks]
;0030   2bytes - count items of memory map below
;0040   memory map list
;   24bytes per item----
;         First uint64_t = Base address
; Second uint64_t = Length of "region" (if this value is 0, ignore the entry)
; Next uint32_t = Region "type"
;   Type 1: Usable (normal) RAM
;   Type 2: Reserved - unusable
;   Type 3: ACPI reclaimable memory
;   Type 4: ACPI NVS memory
;   Type 5: Area containing bad memory
; Next uint32_t = ACPI 3.0 Extended Attributes  bitfield(if 24 bytes are returned, instead of 20)
; Bit 0 of the Extended Attributes indicates if the entire entry should be ignored (if the bit is clear). This is going to be a huge compatibility problem because most current OSs won't read this bit and won't ignore the entry.
; Bit 1 of the Extended Attributes indicates if the entry is non-volatile (if the bit is set) or not. The standard states that "Memory reported as non-volatile may require characterization to determine its suitability for use as conventional RAM."
; The remaining 30 bits of the Extended Attributes are currently undefined.
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

;reset disk
    mov ah,00h  ;reset
    mov dl,90h ; >80h : harddisk
    int 13h     ;disk service
    mov ah,0dh  ;reset
    mov dl,90h ; >80h : harddisk
    int 13h     ;disk service

;call int 15 ,al=0xe820 to get memory map

;           Basic Usage:
; For the first call to the function, 
; point ES:DI at the destination buffer for the list. 
; Clear EBX. 
; Set EDX to the magic number 0x534D4150. 
; Set EAX to 0xE820 (note that the upper 16-bits of EAX should be set to 0). 
; Set ECX to 24. 
; Do an INT 0x15.


; If the first call to the function is successful, 
; EAX will be set to 0x534D4150, 
; and the Carry flag will be clear. 
; EBX will be set to some non-zero value, which must be preserved for the next call to the function. 
; CL will contain the number of bytes actually stored at ES:DI (probably 20).

; For the subsequent calls to the function: 
; increment DI by your list entry size, 
; reset EAX to 0xE820, 
; and ECX to 24. 

; When you reach the end of the list, 
; EBX may reset to 0. 
; If you call the function again with EBX = 0, the list will start over. 
; If EBX does not reset to 0, the function will return with Carry set when you try to access the entry after the last valid entry.

    mov ebx,0
    mov ax,DATASEG
    mov es,ax
    mov ds,ax
    mov ax,40h
    mov di,ax
    mov edx,0x534D4150

    mov word [ds:30h],0;clear count to 0

read_memory_next:
    inc word [ds:30h]
    mov eax,0xe820
    mov ecx,24
    int 15h

    ;if reach end
    ;jnc read_memory_end
    cmp ebx,0
    je  read_memory_end

    ;else loop
    add di,24
    jmp read_memory_next

read_memory_end:

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


set_gdt_end:


;set idt
;---------------------------------------------------------
set_idt:
    lidt [idt_decriptor]
    jmp set_idt_end


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
    ;enable 32-bit
    mov eax,cr0
    or eax,1
    mov cr0,eax

    ;set segment-regs to kernel-data-seg
    mov ax,10h;10 00 0
    mov ss,ax
    mov ds,ax
    mov es,ax
    mov fs,ax
    mov gs,ax
    mov esp,KERNEL_TOTAL_SOZE
    
    ;0x9020:0x1b6
    call 8:0;segment selector : 1 00 0;

    jmp $
    
;delay for I/O
delay:
	dw 0x00eb ;jmp $+2
	ret

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

idt_decriptor:
    dw 0
    dw 0,0

a20error_str:
    db "enable a20 failed! please reboot!",10,13
a20error_str_end:
