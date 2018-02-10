target("kernel")
    set_kind("object")
    add_deps("libs")
    add_deps("boot")
    --source

    add_files("**.cpp")
    
    add_includedirs("$(projectdir)")
    add_includedirs("$(projectdir)/libs")
    add_includedirs("$(projectdir)/kernel")

    after_build(function ()

        os.mkdir("$(buildir)/kernel/kernel/interrupt")
        os.vrun("nasm kernel/interrupt/deal_int.asm -o$(buildir)/kernel/kernel/interrupt/deal_int.o -f elf32")

        -- 链接kernel
        os.vrun("g++ -ggdb3 -o $(buildir)/kernel.o -Wl,-Tkernel/link.lds -static -nostartfiles -m32 -ffreestanding $(buildir)/kernel/**.o $(buildir)/libs/**.o $(buildir)/boot/**.o")

        --调试
        os.vrun("$(projectdir)/scripts/dump.bat")
        os.cp("$(buildir)/kernel.o","$(buildir)/kernel.dbg")

        --转换为elf格式
        os.vrun("objcopy -O elf32-i386 $(buildir)/kernel.o")

        os.vrun("strip $(buildir)/kernel.o")
    end)
