qemu-system-i386 -boot a -fda build/cyOS.img -hda ./hdqemu.raw -gdb tcp::1234 -S -m 1024 -d guest_errors,cpu_reset
