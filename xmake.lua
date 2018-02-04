set_project("cyOS")

set_languages("cxx11")

--include
add_includedirs("$(projectdir)/")
add_includedirs("$(projectdir)/libs/")
add_includedirs("$(projectdir)/kernel/")

--gcc params
add_cxflags("-fpermissive","-W","-Wall","-werror")--dealing compiler error
add_cxflags("-fno-builtin","-fpack-struct=1",
            "-mno-ms-bitfields","-masm=intel")--compiling
add_cxflags("-m32")--platform
add_cxflags("-ggdb3")--debugging

add_subdirs("./boot/")
add_subdirs("./libs/")
add_subdirs("./kernel/")

--set output dirs
set_objectdir("$(buildir)")
set_targetdir("$(buildir)")
set_headerdir("include")

set_strip("all")


add_moduledirs("./")

add_defines("OS_DEBUG")

target("cyOS")
    add_deps("kernel","boot")
    
	on_build(function ()
        import("filelen")
        local kernel_len_512 = math.ceil(filelen.kb("$(buildir)/kernel.bin")*2)
        local boot_len_512 = math.ceil(filelen.kb("$(buildir)/boot.bin")*2)
        
        os.run("dd if=$(buildir)/boot.bin of=$(buildir)/cyOS.img \
            bs=512 count=%d"
            ,boot_len_512)
        os.run("dd if=$(buildir)/kernel.bin of=$(buildir)/cyOS.img \
            bs=512 count=%d seek=%d"
            ,kernel_len_512,boot_len_512)
        cprint("${blue}cyOS has been built successfully")
    end)

    on_clean(function () 
        os.rmdir("build")
    end)

    if is_mode("release") then
        on_run(function ()
            os.run("start ./scripts/run.bat")
        end)
    else
        on_run(function ()
            os.run("start ./scripts/debug.bat")
        end)
    end