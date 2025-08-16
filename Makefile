.PHONY: all clean iso run

OUTDIR=out
KERNEL=$(OUTDIR)/kernel.bin
ISO=bos.iso

# All source files
SRCS = src/kernel.c src/console.c src/gui.c src/fs.c src/shell.c src/editor.c src/game.c src/keyboard.c src/ports.c
OBJS = $(patsubst src/%.c,$(OUTDIR)/%.o,$(SRCS))

all: $(ISO)

# Compile all .c files into .o
$(OUTDIR)/%.o: src/%.c
	mkdir -p $(OUTDIR)
	i686-elf-gcc -ffreestanding -O2 -c $< -o $@ || gcc -ffreestanding -m32 -O2 -c $< -o $@

# Link all object files into kernel.bin
$(KERNEL): $(OBJS) src/linker.ld
	i686-elf-ld -T src/linker.ld -o $(KERNEL) $(OBJS) || ld -m elf_i386 -T src/linker.ld -o $(KERNEL) $(OBJS)

# Create ISO
iso: $(KERNEL)
	@mkdir -p iso/boot/grub
	cp $(KERNEL) iso/boot/kernel.bin
	cp boot/grub/grub.cfg iso/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO) iso 2>/dev/null || (echo "grub-mkrescue failed; ensure grub & xorriso installed" && exit 1)

# Dummy target for make
$(ISO): iso
	@true

# Run in QEMU
run: $(ISO)
	qemu-system-i386 -cdrom $(ISO) -m 512M

# Clean build artifacts
clean:
	rm -rf iso out $(ISO)
