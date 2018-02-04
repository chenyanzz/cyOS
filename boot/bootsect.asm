org 7c00h

SETUPSEG equ 9020h
KERNELSEG equ 1000h

;KERNEL_SIZE_512,SETUP_SIZE_512 --在xmake编译时以命令行-dxxx=xx形式传入

;这个操作系统的最最最开始
;=========================================================

;一点点寄存器初始化工作
cli
mov ax,0x7000
mov ss,ax
mov sp,0xFFFF
jmp boot_main

;主程序部分
;=========================================================
boot_main:

;显示个hello字符串
;---------------------------------------------------------
    ;获取当前光标
    mov bx,0;page
    mov ax,0300h
    int 10h

    ;利用中断打印字符串
    mov ax,cs
    mov es,ax;字符串所在段
    mov ax,hellostr
    mov bp,ax;字符串所在地址
    mov cx,hellostr_end-hellostr;字符串长度
    mov bx,7;颜色（黑底白字）
    mov ax,1301h
    int 10h

load_setup:
    ;读取硬盘
    mov dx,0;diskA head0 磁盘A磁头0
    mov cl,2;第2扇区是setup.o
    mov ax,SETUPSEG
    mov es,ax;read-to segment 读到的段
    mov bx,0;read-to offset 读到的地址
    mov ah,2;int 13,ah=2 -> read from device
    mov al,SETUP_SIZE_512;count sector(512byte)s 扇区计数
    int 13h
    jnc load_setup_end;jump if read success 成功就继续
    jmp load_setup;error->repeat 失败就重复
load_setup_end:

;load system to memory 把整个操作系统读入系统
;---------------------------------------------------------
load_sys:
    ;read disk 读，同上
    mov dh,0
    mov dl,0
    mov ch,0;cylinder
    mov cl,2+SETUP_SIZE_512;sector起始扇区, @note 所以最多只能读256sectors=128KB
    mov ax,KERNELSEG
    mov es,ax;read-to segment
    mov bx,0;read-to offset
    mov al,KERNEL_SIZE_512;count sector(512byte)s
    mov ah,2
    int 13h
    jnc load_setup_end;jump if read success 成功就继续
    jmp load_setup;error->repeat 失败就重复

load_complete:
    ;跳到setup
    jmp SETUPSEG:0


hellostr:
db 10,13,"cyOS is starting",10,13,"loading image...",10,13
hellostr_end:

;ending
;=========================================================
times 510-($-$$) db 0
dw 0xaa55;mbr end MBR记录结尾的可引导扇区标志