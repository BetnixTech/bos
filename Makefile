.PHONY: all clean iso run

OUTDIR=out
KERNEL=$(OUTDIR)/kernel.bin
ISO=bos.iso

all: $(ISO)

$(KERNEL): src/kernel.c src/linker.ld
	mkdir -p $(OUTDIR)
	# You should have a cross-compiler; fallback to native gcc if you haven't set one
	i686-elf-gcc -ffreestanding -O2 -c src/kernel.c -o $(OUTDIR)/kernel.o || gcc -ffreestanding -m32 -O2 -c src/kernel.c -o $(OUTDIR)/kernel.o
	i686-elf-ld -T src/linker.ld -o $(KERNEL) $(OUTDIR)/kernel.o || ld -m elf_i386 -T src/linker.ld -o $(KERNEL) $(OUTDIR)/kernel.o

iso: $(KERNEL)
	@mkdir -p iso/boot/grub
	cp $(KERNEL) iso/boot/kernel.bin
	cp boot/grub/grub.cfg iso/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO) iso 2>/dev/null || (echo "grub-mkrescue failed; ensure grub & xorriso installed" && exit 1)

$(ISO): iso
	# target to satisfy make
	@true

run: $(ISO)
	qemu-system-i386 -cdrom $(ISO) -m 512M

clean:
	rm -rf iso out $(ISO)
