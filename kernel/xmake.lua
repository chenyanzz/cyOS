target("kernel")

    --settings
    set_kind("binary")
    set_targetdir("$(buildir)/")
    set_basename("kernel.o")
    --source
    add_files("**.c","**.cpp")

    for _, dir in ipairs(os.dirs("$(projectdir)/kernel/**")) do add_includedirs(dir) end

    --link params
    add_cxflags("-Wl,-T$(projectdir)/kernel/link.lds,-nostartfiles")
    add_ldflags("-T$(projectdir)/kernel/link.lds","-nostartfiles")
    add_deps("libs")

    --after build
    after_build(function ()

        --disassemble
        os.run("objdump -S $(buildir)/kernel.o -M intel > $(projectdir)/d_kernel.txt")

        --cut into plain instruction
	    os.run("objcopy -O binary kernel.o kernel.bin")

    end)