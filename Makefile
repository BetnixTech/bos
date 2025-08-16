.PHONY: all clean iso run

OUTDIR=out
KERNEL=$(OUTDIR)/kernel.bin
ISO=bos.iso

# Source files
SRCS = src/kernel.c src/console.c src/gui.c src/fs.c src/shell.c src/editor.c \
       src/game.c src/ports.c src/paging.c src/kheap.c src/irq.c src/isr.c \
       src/timer.c src/task.c src/keyboard.c src/mouse.c src/syscall.c src/ata.c

# Assembly files
ASMS = src/isr_stubs.asm src/paging_asm.S

# Object files
OBJS = $(patsubst src/%.c,$(OUTDIR)/%.o,$(SRCS)) \
       $(patsubst src/%.asm,$(OUTDIR)/%.o,$(ASMS))

all: $(ISO)

# Compile C files
$(OUTDIR)/%.o: src/%.c
	mkdir -p $(OUTDIR)
	i686-elf-gcc -ffreestanding -O2 -c $< -o $@

# Compile assembly files
$(OUTDIR)/%.o: src/%.asm
	mkdir -p $(OUTDIR)
	nasm -f elf32 $< -o $@

# Link kernel
$(KERNEL): $(OBJS)
	i686-elf-ld -T src/linker.ld -o $(KERNEL) $(OBJS)

# Create ISO
iso: $(KERNEL)
	@mkdir -p iso/boot/grub
	cp $(KERNEL) iso/boot/kernel.bin
	cp boot/grub/grub.cfg iso/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO) iso 2>/dev/null || (echo "grub-mkrescue failed; ensure grub & xorriso installed" && exit 1)

# Run in QEMU
run: $(ISO)
	qemu-system-i386 -cdrom $(ISO) -m 512M

# Clean build files
clean:
	rm -rf iso out $(ISO)
