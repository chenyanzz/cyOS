function filelen_kb(filename)
    local fh = io.open(filename, "rb")
    local lenb = fh:seek("end")
    fh:close()
    local lenkb=lenb/1024
    if lenb>lenkb*1024 then lenkb = lenkb+1 end
    return lenkb
end