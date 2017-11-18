set_project("cySystem")

set_languages("cxx11")

--include
add_includedirs("$(projectdir)/libs/")
for _, dir in ipairs(os.dirs("$(projectdir)/libs/**")) do add_includedirs(dir) end
add_includedirs("$(projectdir)/kernel/")
for _, dir in ipairs(os.dirs("$(projectdir)/kernel/**")) do add_includedirs(dir) end

add_headers("**.h")

--compile params
add_cxflags("-masm=intel","-W","-fno-builtin")
add_cxflags("-i $(projectdir)/kernel/define.h")
add_cxflags("-i $(projectdir)/kernel/gcc_disable_warnings.h")



add_subdirs("./boot/")
add_subdirs("./libs/")
add_subdirs("./kernel/")


target("sySystem.img")
    add_deps("boot","libs","kernel")
    
	on_build(function ()
        os.run("dd if=$(buildir)/boot.bin of=$(buildir)/cySystem.img bs=512 count=3")
        os.run("dd if=$(buildir)/kernel.bin of=$(buildir)/cySystem.img bs=512 count=100 seek=3")
    end)