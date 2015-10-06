SRCDIR = src
OBJDIR = obj

OBJECTS = $(OBJDIR)/loader.o $(OBJDIR)/io.o $(OBJDIR)/framebuffer.o \
		  $(OBJDIR)/serial.o $(OBJDIR)/kmain.o
CC = gcc
CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
	 -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -c

LDFLAGS = -T link.ld -melf_i386
AS = nasm
ASFLAGS = -f elf

all: kernel.elf

kernel.elf: $(OBJECTS)
	ld $(LDFLAGS) $(OBJECTS) -o $(OBJDIR)/kernel.elf

os.iso: kernel.elf
	cp $(OBJDIR)/kernel.elf iso/boot/kernel.elf
	genisoimage -R                       \
		    -b boot/grub/stage2_eltorito \
		    -no-emul-boot                \
		    -boot-load-size 4            \
		    -A os                        \
		    -input-charset utf8          \
		    -quiet                       \
		    -boot-info-table             \
		    -o $(OBJDIR)/os.iso          \
		    iso

run: os.iso
	bochs -f bochsrc.txt -q

$(OBJDIR)/kmain.o: $(SRCDIR)/kmain.c
	$(CC) $(CFLAGS) $< -o $@

$(OBJDIR)/framebuffer.o: $(SRCDIR)/framebuffer.c
	$(CC) $(CFLAGS) $< -o $@

$(OBJDIR)/serial.o: $(SRCDIR)/serial.c
	$(CC) $(CFLAGS) $< -o $@

$(OBJDIR)/loader.o: $(SRCDIR)/loader.s
	$(AS) $(ASFLAGS) $< -o $@

$(OBJDIR)/io.o: $(SRCDIR)/io.s
	$(AS) $(ASFLAGS) $< -o $@

#%.o: %.c
#	$(CC) $(CFLAGS) $< -o $@
#
#%o: %.s
#	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -rf obj/*.o obj/kernel.elf obj/os.iso bochslog.txt iso/boot/kernel.elf \
		com1.out
