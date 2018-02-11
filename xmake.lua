set_project("cyOS")

set_languages("cxx11")

--include
add_includedirs("$(projectdir)/")
add_includedirs("$(projectdir)/libs/")
add_includedirs("$(projectdir)/kernel/")

--gcc params
add_cxflags(
    "-fpermissive","-W","-Wall",  --error-dealing
    "-fno-builtin","-fpack-struct=1","-nostdlib",
    --"-ffunction-sections","-fdata-sections",
    "-mno-ms-bitfields","-masm=intel",  --compiling
    "-m32", --platform
    "-ggdb3",   --debugging
    {force=true})

add_defines("OS_DEBUG")

add_subdirs("./boot/")
add_subdirs("./libs/")
add_subdirs("./kernel/")

--set output dirs
set_objectdir("$(buildir)")
set_targetdir("$(buildir)")
set_headerdir("include")

add_moduledirs("./")

target("cyOS")
    add_deps("cyOS.iso")

    after_build(function ()
        cprint("${blue}cyOS has been built successfully")
    end)

target("cyOS.iso")
    add_deps("kernel")

	on_build(function ()
        --os.cp("$(buildir)/kernel.o","iso/")
        --os.vrun("ubuntu run grub-mkrescue -o cyOS.iso iso/")
    end)