;setup.asm用来为进入C/C++的操作系统程序做准备
jmp enable_a20

;prepare for & jump to protected mode
;=========================================================
KERNELSEG equ 1000h
DATASEG equ 9000h
SETUPSEG equ 9020h
;KERNEL_SIZE_512 ->later define on cmd -dxxx=xx

AFTER_KERNEL_SIZE equ 1024*1024*10

;enable a20 for memory beyond 1MB 打开A20
;---------------------------------------------------------
enable_a20:
    ;there are three methods 尝试3种方法
    
    ;;BIOS int BIOS调用
    mov ax,0x2401
    int 15h
    call testa20
    ;;port on board 主板端口
    in al,0x92
    or al,2
    and al,0b11111110
    out 0x92,al
    call testa20
    ;;keyboard controller 键盘控制器
    call    empty_8042
    mov     al,0xd1
    out     0x64,al
    call    empty_8042
    mov     al,0xdf
    out     0x60,al
    call    empty_8042
    call testa20

    ;如果都不行就报错然后死这里

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

    ;die 死机进行曲
    jmp $

empty_8042:
    dw    00ebh,00ebh            
    in    al,64h            
    test  al,2
    jnz   empty_8042
    ret

;用来检测是否开启了A20，如果开启，跳转到下面执行，不然返回
testa20:
    ;通过比较0x100001与0x1地址的数据是否一致
    
    mov ax,0
    mov es,ax
    mov ax,0xFFFF
    mov gs,ax
    mov ax,111
    mov bx,233
    mov [es:0x1],ax
    
    ;0xffff:0x11 = 0x100001
    mov [gs:0x11],bx
    cmp [gs:0x11],ax
    jne enable_a20_end
    ret

enable_a20_end:

save_bios_data:
;save bios data to DATASEG: 把BIOS的一些有用的数据存起来（短地址=DATASEG）
;偏移 |  数据解释
;0000   cursor pos 光标位置
;   [0]:col 列
;   [1]:row 行
;0002   extended memory size 扩展内存大小？？exm这是啥
;0004   VGA display mode 显示模式？？exm
;0010   harddisk settings[16byte*2disks] 硬盘设置？？exm
;0030   2bytes - count items of memory map below 内存分布表的项数（从0开始）
;0040   memory map list 内存分布表
;   24bytes per item---- 每项24自己
;       First uint64_t = Base address 内存基址
;       Second uint64_t = Length of "region" (if this value is 0, ignore the entry) 长度
;       Next uint32_t = Region "type" 类型
;           Type 1: Usable (normal) RAM 未使用
;           Type 2: Reserved - unusable 保留/不能用
;           Type 3: ACPI reclaimable memory 什么鬼
;           Type 4: ACPI NVS memory 什么鬼（3和4总之也是不能用，PS.我就没遇见过这两个）
;           Type 5: Area containing bad memory 坏区
; Next uint32_t = ACPI 3.0 Extended Attributes  bitfield(if 24 bytes are returned, instead of 20)
; Bit 0 of the Extended Attributes indicates if the entire entry should be ignored (if the bit is clear). This is going to be a huge compatibility problem because most current OSs won't read this bit and won't ignore the entry.
; Bit 1 of the Extended Attributes indicates if the entry is non-volatile (if the bit is set) or not. The standard states that "Memory reported as non-volatile may require characterization to determine its suitability for use as conventional RAM."
; The remaining 30 bits of the Extended Attributes are currently undefined.
;最后一个32位是各种属性，没什么用
;---------------------------------------------------------

    ;set data segment 设置存储的数据段
    mov ax,DATASEG
    mov ds,ax

    ;下面的一一对应什么的数据分布，不一一注释了

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
    lds si,[ds:4*41h];41h中断向量指向第一块硬盘的设置
    mov es,ax
    mov di,0010h
    mov cx,10h
    rep movsb
    ;disk1
    mov ax,0
    mov ds,ax
    lds si,[ds:4*46h];第二块
    mov es,ax
    mov di,0020h
    mov cx,10h
    rep movsb
    ;clear disk1 if =null 如果没有第二块硬盘就清空
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

;reset disk 重启硬盘，以初始状态进入系统
    mov ah,00h  ;reset
    mov dl,90h ; >80h : harddisk
    int 13h     ;disk service
    mov ah,0dh  ;reset
    mov dl,90h ; >80h : harddisk
    int 13h     ;disk service

;call int 15 ,al=0xe820 to get memory map 获取内存分布表

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
    
    ;初始化准备调用
    mov ebx,0
    mov ax,DATASEG
    mov es,ax
    mov ds,ax
    mov ax,40h
    mov di,ax
    mov edx,0x534D4150;魔法的标识

    mov word [ds:30h],0;clear count to 0 项计数清零

read_memory_next:
    inc word [ds:30h];每次count++
    mov eax,0xe820
    mov ecx,24
    int 15h;获取一个表项

    ;if reach end 如果到结尾了
    ;jnc read_memory_end 跳走
    cmp ebx,0
    je  read_memory_end

    ;else loop 否则循环
    add di,24
    jmp read_memory_next

read_memory_end:

;move binary image from KERNELSEG to 0 把内核移动到内存0位置
;---------------------------------------------------------
move_sys:
;init for copy 准备
mov ax,KERNELSEG
mov ds,ax
mov si,0
mov bx,0
mov es,bx
mov di,0

mov ax,1;num of current kb ax计数拷贝了几KB了
do_move:
    ;一次移动1K
    cmp ax,KERNEL_SIZE_512*2
    jg move_sys_end;if current >limit break
    mov cx,1024;move 1k
    rep movsb
    inc ax;curr kb++

    jmp do_move;loop

move_sys_end:

;set gdt 加载GDT，全局描述符表
;---------------------------------------------------------
set_gdt:
    mov ax,SETUPSEG     ;0x9028d
    mov ds,ax
    lgdt [gdt_decriptor]
    jmp set_gdt_end     ;0x90297

set_gdt_end:


;set idt 加载空的IDT，中断描述符表
;---------------------------------------------------------
set_idt:
    lidt [idt_decriptor]
    jmp set_idt_end


set_idt_end:

;program for the interrupt controller 初始化中断控制器
;---------------------------------------------------------
init_8259A:;such a messy procedure... 乱七八糟无理取闹不用管他

    ;this subprocess is copied from linux-0.1

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

;jump to protected mode 调到保护模式
;and jump to kernel 进入C/C++内核
;---------------------------------------------------------
jump_to_protected_mode:
    ;enable 32-bit 开启32位模式
    mov eax,cr0
    or eax,1
    mov cr0,eax

    ;set segment-regs to kernel-data-seg 设置内核段
    mov ax,10h;10 00 0
    mov ss,ax
    mov ds,ax
    mov es,ax
    mov fs,ax
    mov gs,ax
    mov esp,(KERNEL_SIZE_512*512)+AFTER_KERNEL_SIZE;把栈顶设在kernel的尾巴上（栈往前存储）
    
    ;0x9020:0x1b6
    call 8:0;segment selector : 1 00 0; 调到C的main函数

    jmp $;main退出了就死循环死在这里，多半是出错了
    
;delay for I/O
delay:
	dw 0x00eb ;jmp $+2
	ret

;GDTR的内容
gdt_decriptor:      ;48bits=6bytes
    dw 2048    ;size 256*(8 per)
    dd 0x90200+gdt_data;gdt addr

;GDT表
gdt_data:
    ;null for first 第0项置空，实现空指针报错
    dw 0,0,0,0
    
    ;kernel code segment 第一项是内核代码段
    dw 0xFFFF;limit
    dw 0;base addr
    db 0
    db 10011010b;attr
    db 11001111b
    db 0;base addr
    
    ;kernel data segment 第二项内核数据段
    dw 0xFFFF;limit
    dw 0;base addr
    db 0;base addr
    db 10010010b;attr
    db 11001111b
    db 0;base addr

;IDTR的内容 先置空，到时候在C/C++层再做
idt_decriptor:
    dw 0
    dw 0,0

a20error_str:
    db "enable a20 failed! please reboot!",10,13
a20error_str_end:
