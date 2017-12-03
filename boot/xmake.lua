target("boot")
    add_deps("kernel") --to get the size of kernel and write it in asm

    on_build(function ()
        -- require("filelen")
        -- local kernel_len_kb = filelen_kb("$(buildir)/kernel.bin")
        -- local setup_len_kb = filelen_kb("$(buildir)/boot/setup.o")
        local kernel_len_kb = 10
        local setup_len_kb = 2
        os.mkdir("$(buildir)/boot")

        cprint("${blue}building boot/setup.o")
        os.vrun("nasm boot/setup.asm -o $(buildir)/boot/setup.o \
         -dKERNEL_SIZE_KB=%d",kernel_len_kb)

        cprint("${blue}building boot/bootsect.o")
        os.vrun("nasm boot/bootsect.asm -o $(buildir)/boot/bootsect.o \
        -dKERNEL_SIZE_KB=%d -dSETUP_SIZE_KB=%d",kernel_len_kb,setup_len_kb)
        
        cprint("${blue}building boot.bin")
        os.vrun("dd if=$(buildir)/boot/bootsect.o of=$(buildir)/boot.bin \
            bs=512 count=1 conv=notrunc,sync")
	    os.vrun("dd if=$(buildir)/boot/setup.o of=$(buildir)/boot.bin \
            bs=512 count=%d seek=1 conv=notrunc,sync"
            ,setup_len_kb*2+1) -- +1 to make file size a full kb

    end)

    on_clean(function () 
        os.rm("$(buildir)/boot.bin")
        os.rm("$(buildir)/boot/*")
    end)