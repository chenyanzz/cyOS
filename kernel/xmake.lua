-- target("kernel.bin")

--     --settings
--     set_kind("binary")

--     --source
--     add_files("**.c","**.cpp")

--     for _, dir in ipairs(os.dirs("$(projectdir)/kernel/**")) do add_includedirs(dir) end

--     --link params
--     add_cxflags("-Wl,-T$(projectdir)/kernel/link.lds","-nostartfiles","-e ___main",{force=true})
--     add_ldflags("-Wl,-T$(projectdir)/kernel/link.lds","-nostartfiles","-e ___main",{force=true})
--     add_links("stdc++")
--     --add_deps("libs")

--     --after build
--     after_build(function ()

--         --disassemble
--         os.run("objdump -S $(buildir)/libkernel.a -M intel > $(projectdir)/d_kernel.txt")

--         --cut into plain code
-- 	    os.run("objcopy -O binary $(buildir)/libkernel.a $(buildir)/kernel.bin")

--     end)
target("kernel")
    set_kind("object")
    add_deps("libs")
    --source
    add_files("**.cpp","**.c")
    -- add_cxflags("-nostartfiles","-m32","-fno-builtin",{force=true})

    for _, dir in ipairs(os.dirs("$(projectdir)/kernel/**")) do add_includedirs(dir) end

    after_build(function ()

        os.vrun("nasm kernel/deal_int.asm -o$(buildir)/kernel/deal_int.o -p kernel/int_handler.inc -f elf32")

        -- 链接kernel
        os.vrun("gcc -ggdb3 $(buildir)/kernel/**.o $(buildir)/libs/**.o -o $(buildir)/kernel.o -Wl,-Tkernel/link.lds,-e,_start -lstdc++ -nostartfiles -nostdinc -m32 -static-libstdc++ -static -fno-builtin")

        -- 提取调试信息文件和纯二进制数据
        os.vrun("objcopy --only-keep-debug $(buildir)/kernel.o $(buildir)/kernel.dbg")
	    os.vrun("objcopy -O binary $(buildir)/kernel.o $(buildir)/kernel.bin")

        -- 导出反汇编信息
        local ret,_ = os.iorun("objdump -S build/kernel.o -M intel")
        io.writefile("d_kernel.txt",ret)
    end)
