objdump -S build/kernel.o -M intel > d_kernel.txt
objdump -S build/kernel/kernel/main.cpp.o -M intel > d_main.txt