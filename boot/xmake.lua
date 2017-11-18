target("boot")
    local dir=os.curdir()
    
    on_build(function () 
        os.run("make -C "..dir.." boot.bin")
    end)

    on_clean(function () 
        os.run("make -C "..dir.." clean")
    end)