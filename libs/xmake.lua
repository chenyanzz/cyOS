target("libs")

    --settings
    set_kind("static")
    set_targetdir("$(buildir)/obj")
    set_basename("libs")

    --source
    add_files("**.c","**.cpp")