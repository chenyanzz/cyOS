target("libs")

    --settings
    set_kind("static")
    set_targetdir("$(buildir)/obj")

    --source
    add_files("**.c","**.cpp")