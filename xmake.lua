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

add_cflags("-D LANG_C")
add_cxxflags("-D LANG_CPP")

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
        -- import("filelen")
        -- local kernel_len_kb = filelen_kb("$(buildir)/kernel.bin")
        -- local boot_len_kb = filelen_kb("$(buildir)/boot.bin")
        local kernel_len_kb = 10
        local boot_len_kb = 3
        os.vrun("dd if=$(buildir)/boot.bin of=$(buildir)/cyOS.img \
            bs=1024 count=%d conv=notrunc,sync"
            ,boot_len_kb)
        os.vrun("dd if=$(buildir)/kernel.bin of=$(buildir)/cyOS.img \
            bs=1024 count=%d seek=%d conv=notrunc,sync"
            ,kernel_len_kb,boot_len_kb)
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