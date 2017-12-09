set_project("cyOS")

set_languages("cxx11")

--include
add_includedirs("$(projectdir)/libs/")
for _, dir in ipairs(os.dirs("$(projectdir)/libs/**")) do add_includedirs(dir) end
add_includedirs("$(projectdir)/kernel/")
for _, dir in ipairs(os.dirs("$(projectdir)/kernel/**")) do add_includedirs(dir) end

--compile params
add_cxflags("-nostdinc","-masm=intel","-W","-fno-builtin")
add_cxflags("-include $(projectdir)/kernel/define.h")
add_cxflags("-include $(projectdir)/kernel/gcc_disable_warnings.h")

add_subdirs("./boot/")
add_subdirs("./libs/")
add_subdirs("./kernel/")

set_objectdir("$(buildir)")
set_targetdir("$(buildir)")
set_headerdir("include")

set_strip("all")


target("cyOS")
    add_deps("kernel","boot")


	on_build(function ()
        import("filelen")
        -- local kernel_len_kb = filelen_kb("$(buildir)/kernel.bin")
        -- local boot_len_kb = filelen_kb("$(buildir)/boot.bin")
        local kernel_len_512 = 10
        local boot_len_512 = 2
        os.vrun("dd if=$(buildir)/boot.bin of=$(buildir)/cyOS.img \
            bs=512 count=%d"
            ,boot_len_512)
        os.vrun("dd if=$(buildir)/kernel.bin of=$(buildir)/cyOS.img \
            bs=512 count=%d seek=%d"
            ,kernel_len_512,boot_len_512)
        cprint("${blue}cyOS has been built successfully")
    end)

    on_clean(function () 
        os.rmdir("build")
    end)

    if is_mode("release") then
        on_run(function ()
            os.run("./run.bat")
        end)
    else
        on_run(function ()
            os.run("./debug.bat")
        end)
    end