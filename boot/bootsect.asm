org 7c00h

SETUPSEG equ 9020h
KERNELSEG equ 1000h
KERNEL_SIZE_KB equ 12
SETUP_SIZE_KB equ 1

;program starts here
;=========================================================
jmp boot_main

str:
db 10,13,"cyOS",10,13,"loading image...",10,13
strend:

;bootloader main
;=========================================================
boot_main:

;display str
;---------------------------------------------------------
    ;get current cursor position
    mov bx,0;page
    mov ax,0300h
    int 10h

    ;write str
    mov ax,cs
    mov es,ax;str segment
    mov ax,str
    mov bp,ax;str addr
    mov cx,strend-str;str len
    mov bx,7;color
    mov ax,1301h
    int 10h

load_setup:
    ;read disk
    mov dx,0;diskA head0
    mov cl,2;sector
    mov ax,SETUPSEG
    mov es,ax;read-to segment
    mov bx,0;read-to offset
    mov ah,2;start sector
    mov al,SETUP_SIZE_KB*2;count sector(512byte)s
    int 13h
    jnc load_setup_end;jump if read success
    jmp load_setup;error->repeat
load_setup_end:

;load system to memory
;---------------------------------------------------------
load_sys:
    ;read disk
    mov dx,0;diskA head0
    mov ch,0
    mov cl,2+SETUP_SIZE_KB*2;sector
    mov ax,KERNELSEG
    mov es,ax;read-to segment
    mov bx,0;read-to offset
    mov al,KERNEL_SIZE_KB*2;count sector(512byte)s
    mov ah,2
    int 13h
    jnc load_success;jump if read success
    jmp load_sys;error->repeat
load_success:
    jmp SETUPSEG:0
;ending
;=========================================================
times 510-($-$$) db 0
dw 0xaa55;mbr end