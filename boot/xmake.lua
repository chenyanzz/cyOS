target("boot")
    set_kind("object")

    --source
    add_files("**.cpp")

    add_includedirs("$(projectdir)")
    add_includedirs("$(projectdir)/libs")
    add_includedirs("$(projectdir)/kernel")