bootsect.o: bootsect.asm
	nasm bootsect.asm -o $(buildir)/bootsect.o

setup.o: setup.asm
	nasm setup.asm -o $(buildir)/setup.o

boot.bin: bootsect.o setup.o
	dd if=$(buildir)/bootsect.o of=$(buildir)/boot.bin bs=512 count=1
	dd if=$(buildir)/setup.o of=$(buildir)/boot.bin bs=512 count=2 seek=1

clean:
	-@del *.bin
	-@del *.o