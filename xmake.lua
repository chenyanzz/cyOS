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

target("cyOS")
    add_deps("boot","kernel")
	on_build(function ()
        os.vrun("dd if=$(buildir)/boot.bin of=$(buildir)/cySystem.img bs=512 count=3")
        os.vrun("dd if=$(buildir)/kernel.bin of=$(buildir)/cySystem.img bs=512 count=100 seek=3")
        cprint("${blue}cyOS has been built successfully")
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