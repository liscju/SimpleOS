OBJECTS = obj/loader.o obj/io.o obj/framebuffer.o obj/kmain.o
CC = gcc
CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
	 -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -c

LDFLAGS = -T link.ld -melf_i386
AS = nasm
ASFLAGS = -f elf

all: kernel.elf

kernel.elf: $(OBJECTS)
	ld $(LDFLAGS) $(OBJECTS) -o obj/kernel.elf

os.iso: kernel.elf
	cp obj/kernel.elf iso/boot/kernel.elf
	genisoimage -R                       \
		    -b boot/grub/stage2_eltorito \
		    -no-emul-boot                \
		    -boot-load-size 4            \
		    -A os                        \
		    -input-charset utf8          \
		    -quiet                       \
		    -boot-info-table             \
		    -o obj/os.iso                \
		    iso

run: os.iso
	bochs -f bochsrc.txt -q

obj/kmain.o: src/kmain.c
	$(CC) $(CFLAGS) $< -o $@

obj/framebuffer.o: src/framebuffer.c
	$(CC) $(CFLAGS) $< -o $@

obj/loader.o: src/loader.s
	$(AS) $(ASFLAGS) $< -o $@

obj/io.o: src/io.s
	$(AS) $(ASFLAGS) $< -o $@

#%.o: %.c
#	$(CC) $(CFLAGS) $< -o $@
#
#%o: %.s
#	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -rf obj/*.o obj/kernel.elf obj/os.iso bochslog.txt iso/boot/kernel.elf
