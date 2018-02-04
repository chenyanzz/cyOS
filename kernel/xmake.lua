target("kernel")
    set_kind("object")
    add_deps("libs")
    --source
    add_files("**.cpp")
    
    add_includedirs("$(projectdir)")
    add_includedirs("$(projectdir)/libs")

    after_build(function ()

        os.mkdir("$(buildir)/kernel/kernel/interrupt")
        os.vrun("nasm kernel/interrupt/deal_int.asm -o$(buildir)/kernel/kernel/interrupt/deal_int.o -f elf32")

        -- 链接kernel
        os.vrun("g++ -ggdb3 -o $(buildir)/kernel.o -Wl,-Tkernel/link.lds -static -nostartfiles -m32 -ffreestanding $(buildir)/kernel/**.o $(buildir)/libs/**.o")

        -- 提取调试信息文件和纯二进制数据
        os.vrun("objcopy --only-keep-debug $(buildir)/kernel.o $(buildir)/kernel.dbg")
        os.vrun("strip -s -o $(buildir)/kernel.stripped $(buildir)/kernel.o")
	    os.vrun("objcopy -O binary $(buildir)/kernel.stripped $(buildir)/kernel.bin")


        -- 导出反汇编信息
        local ret,_ = os.iorun("objdump -S build/kernel.o -M intel")
        io.writefile("d_kernel.txt",ret)
    end)
