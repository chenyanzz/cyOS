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
    add_files("**.cpp")
    -- add_cxflags("-nostartfiles","-m32","-fno-builtin",{force=true})

    for _, dir in ipairs(os.dirs("$(projectdir)/kernel/**")) do add_includedirs(dir) end

    after_build(function ()
        os.vrun("gcc $(buildir)/kernel/**.o $(buildir)/libs/**.o -o $(buildir)/kernel.o -Wl,-Tkernel/link.lds,-e,_start -lstdc++ -nostartfiles -m32 -static-libstdc++ -static -fno-builtin")

        --disassemble
        --os.vrun("objdump -S $(buildir)/kernel.o -M intel|echo > d_kernel.txt")
        --os.vrun("objdump -S $(buildir)/kernel/kernel/main.cpp.o -M intel|echo > d_main.txt")
        --cut into plain code
	    os.vrun("objcopy -O binary $(buildir)/kernel.o $(buildir)/kernel.bin")

    end)
