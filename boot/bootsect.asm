org 7c00h

SETUPSEG equ 9020h
KERNELSEG equ 1000h

;KERNEL_SIZE_512,SETUP_SIZE_512 --later define on cmd -dxxx=xx

;program starts here
;=========================================================
jmp boot_main

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
    mov ax,hellostr
    mov bp,ax;str addr
    mov cx,hellostr_end-hellostr;str len
    mov bx,7;color
    mov ax,1301h
    int 10h

load_setup:
    ;read disk
    mov dx,0;diskA head0
    mov cl,2;start sector
    mov ax,SETUPSEG
    mov es,ax;read-to segment
    mov bx,0;read-to offset
    mov ah,2;int 13,ah=2 -> read from device
    mov al,SETUP_SIZE_512;count sector(512byte)s
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
    mov cl,2+SETUP_SIZE_512;sector
    mov ax,KERNELSEG
    mov es,ax;read-to segment
    mov bx,0;read-to offset
    mov al,KERNEL_SIZE_512;count sector(512byte)s
    mov ah,2
    int 13h
    jnc load_success;jump if read success

    ;cuz kernel size is changing
    ;jmp load_sys;error->repeat
load_success:
    jmp SETUPSEG:0


hellostr:
db 10,13,"cyOS is starting",10,13,"loading image...",10,13
hellostr_end:

;ending
;=========================================================
times 510-($-$$) db 0
dw 0xaa55;mbr end