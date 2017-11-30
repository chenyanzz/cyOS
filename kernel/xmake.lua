-- target("kernel")

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

    --source
    add_files("**.c","**.cpp")

    for _, dir in ipairs(os.dirs("$(projectdir)/kernel/**")) do add_includedirs(dir) end

    after_build(function () 
        os.run("gcc build\\.objs\\kernel\\kernel\\main.cpp.o build\\.objs\\kernel\\kernel\\printf.cpp.o build\\.objs\\kernel\\kernel\\gdt.cpp.o build\\.objs\\kernel\\kernel\\asm.cpp.o build\\.objs\\kernel\\kernel\\thread\\thread.cpp.o build\\.objs\\kernel\\kernel\\thread\\test.cpp.o build\\.objs\\kernel\\kernel\\memory\\page.cpp.o build\\.objs\\kernel\\kernel\\idt.cpp.o  build\\obj\\liblibs.a -o build/kernel.o -Wl,-Tkernel/link.lds -lstdc++ -nostartfiles -e _main -m32")

        --disassemble
        os.run("echo (objdump -S build/kernel.o -M intel) > d_kernel.txt")

        --cut into plain code
	    os.run("objcopy -O binary build/kernel.o build/kernel.bin")

    end)
