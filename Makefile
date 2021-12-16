# directories
SRCDIR := src
BUILDDIR := build
IMGDIR := img
LINKER_SCRIPT := $(SRCDIR)/kernel/linker.ld

# make an extra copy of the image so that it can be viewed by a hex editor during emulation
OUT_IMG := $(IMGDIR)/disk.img
IMG_COPY := $(IMGDIR)/disk_copy.img

# flags
CFLAGS := -ffreestanding -Wall -Wextra -Wpedantic -std=gnu11 -O3
LINKER_FLAGS := -ffreestanding -nostdlib -lgcc -Xlinker -Map=$(BUILDDIR)/kernel.map

# final os components
BOOTSECTOR := $(BUILDDIR)/boot/bootsector.bin 
BOOTLOADER := $(BUILDDIR)/boot/bootloader.bin
KERNEL := $(BUILDDIR)/kernel/kernel.bin

# kernel files
_C_OBJ_FILES := main.o terminal.o gdt.o idt.o exception.o pic.o irq.o physalloc.o paging.o
_ASM_OBJ_FILES := init.o ioport.o gdt.o idt.o exception.o irq.o paging.o tss.o usermode.o
C_OBJ_FILES = $(patsubst %,$(BUILDDIR)/kernel/c/%,$(_C_OBJ_FILES))
ASM_OBJ_FILES = $(patsubst %,$(BUILDDIR)/kernel/asm/%,$(_ASM_OBJ_FILES))

# declare a couple targets so they don't get overwritten by files
.PHONY: eggkernel clean

# clean, build image, copy image
eggkernel: clean $(OUT_IMG)
	cp $(OUT_IMG) $(IMG_COPY)

# assemble OS components into a flat image
$(OUT_IMG): $(BOOTSECTOR) $(BOOTLOADER) $(KERNEL)
	dd if=$(BOOTSECTOR) of=$@
	dd if=$(BOOTLOADER) of=$@ seek=1 bs=512
	dd if=$(KERNEL) of=$@ seek=3 bs=512

# build bootsector and bootloader
$(BUILDDIR)/boot/%.bin: $(SRCDIR)/boot/%.asm
	nasm -f bin $< -o $@

# compile C parts of kernel
$(BUILDDIR)/kernel/c/%.o: $(SRCDIR)/kernel/%.c
	i686-elf-gcc -c $< -o $@ $(CFLAGS)

# assemble asm parts of kernel
$(BUILDDIR)/kernel/asm/%.o: $(SRCDIR)/kernel/%.asm
	nasm -f elf $< -o $@ -i $(SRCDIR)/kernel

# link kernel
$(KERNEL): $(C_OBJ_FILES) $(ASM_OBJ_FILES)
	i686-elf-gcc -T $(LINKER_SCRIPT) -o $(KERNEL) $(LINKER_FLAGS) $^

clean:
	rm -rf $(BUILDDIR)
	rm -rf $(IMGDIR)
	mkdir -p $(BUILDDIR)/kernel/c $(BUILDDIR)/kernel/asm $(BUILDDIR)/boot $(IMGDIR)