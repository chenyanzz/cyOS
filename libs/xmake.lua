target("libs")

    --settings
    set_kind("object")
    
    add_cxflags("-ggdb3")

    --source
    add_files("**.cpp")

    set_headerdir("include/libs")
    add_headers("libs/(**.h)")
