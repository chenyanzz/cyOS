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
    set_kind("static")
    add_deps("libs")
    --source
    add_files("**.c","**.cpp")

    for _, dir in ipairs(os.dirs("$(projectdir)/kernel/**")) do add_includedirs(dir) end

    after_build(function ()
        os.vrun("gcc $(buildir)/libkernel.a $(buildir)/liblibs.a -o $(buildir)/kernel.o -Wl,-Tkernel/link.lds -lstdc++ -nostartfiles -e _main -m32")

        --disassemble
        os.vrun("echo (objdump -S $(buildir)/kernel.o -M intel) > d_kernel.txt")

        --cut into plain code
	    os.vrun("objcopy -O binary $(buildir)/kernel.o $(buildir)/kernel.bin")

    end)
