ASM=nasm
CC=g++
LD=ld

CFLAGS=-m32 -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore
LDFLAGS=-melf_i386
ASFLAGS=-f elf32

all: os.iso

boot.o: boot.asm
	$(ASM) $(ASFLAGS) -o $@ $<

kernel.o: kernel.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

kernel.bin: boot.o kernel.o linker.ld
	$(LD) $(LDFLAGS) -T linker.ld -o $@ boot.o kernel.o

os.iso: kernel.bin
	mkdir -p iso/boot/grub
	cp kernel.bin iso/boot/
	echo 'set timeout=0' > iso/boot/grub/grub.cfg
	echo 'set default=0' >> iso/boot/grub/grub.cfg
	echo 'menuentry "MashallyOS" {' >> iso/boot/grub/grub.cfg
	echo ' multiboot /boot/kernel.bin' >> iso/boot/grub/grub.cfg
	echo '}' >> iso/boot/grub/grub.cfg
	grub-mkrescue -o os.iso iso
	rm -rf iso

clean:
	rm -f *.o *.bin *.iso
	rm -rf iso

run: os.iso
	qemu-system-i386 -cdrom os.iso

