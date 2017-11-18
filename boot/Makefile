bootsect.o: bootsect.asm
	nasm bootsect.asm -o bootsect.o

setup.o: setup.asm
	nasm setup.asm -o setup.o

boot.bin: bootsect.o setup.o
	dd if=bootsect.o of=boot.bin bs=512 count=1
	dd if=setup.o of=boot.bin bs=512 count=2 seek=1

clean:
	-@del *.bin
	-@del *.o