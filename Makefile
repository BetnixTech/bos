.PHONY: all clean iso run

OUTDIR=out
KERNEL=$(OUTDIR)/kernel.bin
ISO=bos.iso

SRCS = src/kernel.c src/console.c
ASMS = src/multiboot.s

OBJS = $(patsubst src/%.c,$(OUTDIR)/%.o,$(SRCS)) \
       $(patsubst src/%.s,$(OUTDIR)/%.o,$(ASMS))

all: $(ISO)

$(OUTDIR)/%.o: src/%.c
	mkdir -p $(OUTDIR)
	i686-elf-gcc -ffreestanding -m32 -O2 -c $< -o $@ || gcc -ffreestanding -m32 -O2 -c $< -o $@

$(OUTDIR)/%.o: src/%.s
	mkdir -p $(OUTDIR)
	nasm -f elf32 $< -o $@

$(KERNEL): $(OBJS)
	i686-elf-ld -m elf_i386 -T src/linker.ld -o $(KERNEL) $(OBJS) || \
	ld -m elf_i386 -T src/linker.ld -o $(KERNEL) $(OBJS)

iso: $(KERNEL)
	@mkdir -p iso/boot/grub
	cp $(KERNEL) iso/boot/kernel.bin
	cp boot/grub/grub.cfg iso/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO) iso 2>/dev/null || (echo "grub-mkrescue failed; ensure grub & xorriso installed" && exit 1)

$(ISO): iso
	@true

run: $(ISO)
	qemu-system-i386 -cdrom $(ISO) -m 256M

clean:
	rm -rf iso out $(ISO)
