.PHONY: all clean iso run

OUTDIR=out
KERNEL=$(OUTDIR)/kernel.bin
ISO=bos.iso

# C source files
SRCS = src/kernel.c src/console.c src/gui.c src/fs.c src/shell.c src/editor.c src/game.c \
       src/keyboard.c src/mouse.c src/task.c src/timer.c
OBJS = $(patsubst src/%.c,$(OUTDIR)/%.o,$(SRCS))

# Assembly source files
ASMS = src/task_switch.asm
ASMOBJS = $(patsubst src/%.asm,$(OUTDIR)/%.o,$(ASMS))

all: $(ISO)

$(OUTDIR)/%.o: src/%.c
	mkdir -p $(OUTDIR)
	i686-elf-gcc -ffreestanding -m32 -O2 -c $< -o $@ || gcc -ffreestanding -m32 -O2 -c $< -o $@

$(OUTDIR)/%.o: src/%.asm
	mkdir -p $(OUTDIR)
	nasm -f elf32 $< -o $@

$(KERNEL): $(OBJS) $(ASMOBJS) src/linker.ld
	i686-elf-ld -T src/linker.ld -o $(KERNEL) $(OBJS) $(ASMOBJS) || \
	ld -m elf_i386 -T src/linker.ld -o $(KERNEL) $(OBJS) $(ASMOBJS)

iso: $(KERNEL)
	@mkdir -p iso/boot/grub
	cp $(KERNEL) iso/boot/kernel.bin
	cp boot/grub/grub.cfg iso/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO) iso 2>/dev/null || (echo "grub-mkrescue failed; ensure grub & xorriso installed" && exit 1)

$(ISO): iso
	@true

run: $(ISO)
	qemu-system-i386 -cdrom $(ISO) -m 512M

clean:
	rm -rf iso out $(ISO)
