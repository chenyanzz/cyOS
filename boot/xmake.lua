target("boot")
    on_build(function ()
        os.mkdir("$(buildir)/bootsect.o")

        cprint("${blue}building boot.bin")
        os.vrun("nasm boot/bootsect.asm -o $(buildir)/boot/bootsect.o")

        cprint("${blue}building boot/setup.o")
        os.vrun("nasm boot/setup.asm -o $(buildir)/boot/setup.o")
        
        cprint("${blue}building boot.bin")
        os.vrun("dd if=$(buildir)/boot/bootsect.o of=$(buildir)/boot.bin bs=512 count=1")
	    os.vrun("dd if=$(buildir)/boot/setup.o of=$(buildir)/boot.bin bs=512 count=2 seek=1")
    end)

    on_clean(function () 
        os.rm("$(buildir)/boot.bin")
        os.rm("$(buildir)/boot/*")
    end)