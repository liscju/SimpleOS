Simple operating system to learn sth about it :P

VIM conf:
- setting nasm syntax - :set syntax=nasm

Building:

nasm -f elf32 loader.s
ld -T link.ld -melf_i386 loader.o -o kernel.elf
cp kernel.elf iso/boot
genisoimage -R -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 -A os -input-charset utf8 -quiet -boot-info-table -o os.iso iso

Running:
bochs -f bochsrc.txt -q
