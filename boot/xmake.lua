target("boot")
    add_deps("kernel") --to get the size of kernel and write it in asm

    on_build(function ()
        import("filelen")
        local kernel_len_512 = math.ceil(filelen.kb("$(buildir)/kernel.bin")*2)
        os.mkdir("$(buildir)/boot")

        -- cprint("${blue}building boot/setup.o")
        os.vrun("nasm boot/setup.asm -o $(buildir)/boot/setup.o \
         -dKERNEL_SIZE_512=%d",
         kernel_len_512)

        local setup_len_512 = math.ceil(filelen.kb("$(buildir)/boot/setup.o")*2)
        -- cprint("${blue}building boot/bootsect.o")
        os.vrun("nasm boot/bootsect.asm -o $(buildir)/boot/bootsect.o \
         -dKERNEL_SIZE_512=%d -dSETUP_SIZE_512=%d",
         kernel_len_512,setup_len_512)
        
        -- cprint("${blue}building boot.bin")
        os.run("dd if=$(buildir)/boot/bootsect.o of=$(buildir)/boot.bin \
         bs=512 count=1")
	    os.run("dd if=$(buildir)/boot/setup.o of=$(buildir)/boot.bin \
         bs=512 count=%d seek=1",
         setup_len_512) -- +1 to make file size a full kb
    end)

    on_clean(function () 
        os.rm("$(buildir)/boot.bin")
        os.rm("$(buildir)/boot/*")
    end)