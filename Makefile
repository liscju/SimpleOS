OBJECTS = loader.o io.o framebuffer.o kmain.o
CC = gcc
CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
	 -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -c

LDFLAGS = -T link.ld -melf_i386
AS = nasm
ASFLAGS = -f elf

all: kernel.elf

kernel.elf: $(OBJECTS)
	ld $(LDFLAGS) $(OBJECTS) -o kernel.elf

os.iso: kernel.elf
	cp kernel.elf iso/boot/kernel.elf
	genisoimage -R                           \
		    -b boot/grub/stage2_eltorito \
		    -no-emul-boot                \
		    -boot-load-size 4            \
		    -A os                        \
		    -input-charset utf8          \
		    -quiet                       \
		    -boot-info-table             \
		    -o os.iso                    \
		    iso

run: os.iso
	bochs -f bochsrc.txt -q

kmain.o: src/kmain.c
	$(CC) $(CFLAGS) $< -o $@

framebuffer.o: src/framebuffer.c
	$(CC) $(CFLAGS) $< -o $@

loader.o: src/loader.s
	$(AS) $(ASFLAGS) $< -o $@

io.o: src/io.s
	$(AS) $(ASFLAGS) $< -o $@

#%.o: %.c
#	$(CC) $(CFLAGS) $< -o $@
#
#%o: %.s
#	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -rf *.o kernel.elf os.iso *.o bochslog.txt iso/boot/kernel.elf
