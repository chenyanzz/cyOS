target("kernel")
    set_kind("object")
    add_deps("libs")
    --source
    add_files("**.cpp","**.c")
    
    add_includedirs("$(projectdir)")
    add_includedirs("$(projectdir)/libs")

    after_build(function ()

        os.mkdir("$(buildir)/kernel/kernel/interrupt")
        os.vrun("nasm kernel/interrupt/deal_int.asm -o$(buildir)/kernel/kernel/interrupt/deal_int.o -p kernel/interrupt/int_handler.inc -f elf32")

        -- 链接kernel
        os.vrun("gcc -ggdb3 $(buildir)/kernel/**.o $(buildir)/libs/**.o -o $(buildir)/kernel.o -Wl,-Tkernel/link.lds,-e,_start -lstdc++ -nostartfiles -m32 -static-libstdc++ -static -lm -fno-builtin")

        -- 提取调试信息文件和纯二进制数据
        os.vrun("objcopy --only-keep-debug $(buildir)/kernel.o $(buildir)/kernel.dbg")
	    os.vrun("objcopy -O binary $(buildir)/kernel.o $(buildir)/kernel.bin")

        -- 导出反汇编信息
        local ret,_ = os.iorun("objdump -S build/kernel.o -M intel")
        io.writefile("d_kernel.txt",ret)
    end)
