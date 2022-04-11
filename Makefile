# directories
SRCDIR := ./src
BUILDDIR := ./build
IMGDIR := ./img
LOADER_LINKER_SCRIPT := $(SRCDIR)/bootloader/linker.ld
KERNEL_LINKER_SCRIPT := $(SRCDIR)/kernel/linker.ld

# make an extra copy of the image so that it can be viewed by a hex editor during emulation
OUT_IMG := $(IMGDIR)/disk.img
IMG_COPY := $(IMGDIR)/disk_copy.img
VMDK_IMG := $(IMGDIR)/disk.vmdk

# flags
CFLAGS := -ffreestanding -Wall -Wextra -Wpedantic -std=gnu17 -g -O2
LINK_FLAGS := -ffreestanding -nostdlib -lgcc
KERNEL_LINK_FLAGS := -Xlinker -Map=$(BUILDDIR)/kernel.map

# final os components
BOOTSECTOR := $(BUILDDIR)/bootsector.bin 
BOOTLOADER := $(BUILDDIR)/bootloader.bin
KERNEL := $(BUILDDIR)/kernel/kernel.bin
KERNEL_ELF := $(BUILDDIR)/kernel/kernel.elf
KERNEL_SYMBOLS := $(IMGDIR)/kernel.sym

# object files
_C_OBJ_FILES := main.o terminal.o gdt.o idt.o exception.o pic.o irq.o physalloc.o paging.o print.o
_ASM_OBJ_FILES := init.o ioport.o gdt.o idt.o exception.o irq.o paging.o tss.o 
_BOOTLOADER_OBJ_FILES := bootloader.o a20.o e820.o pmode.o

C_OBJ_FILES = $(patsubst %,$(BUILDDIR)/kernel/c/%,$(_C_OBJ_FILES))
ASM_OBJ_FILES = $(patsubst %,$(BUILDDIR)/kernel/asm/%,$(_ASM_OBJ_FILES))
BOOTLOADER_OBJ_FILES = $(patsubst %,$(BUILDDIR)/bootloader/%,$(_BOOTLOADER_OBJ_FILES))

# declare a couple targets so they don't get overwritten by files
.PHONY: flat vmdk clean

# clean, build image, copy image
flat: $(OUT_IMG)
	cp $(OUT_IMG) $(IMG_COPY)
	
vmdk: $(OUT_IMG)
	VBoxManage convertfromraw $(OUT_IMG) $(VMDK_IMG) --format VMDK --uuid "a3a4a2ff-0512-4099-a7d2-c20ba3799a92"

# assemble OS components into a flat image
$(OUT_IMG): clean $(BOOTSECTOR) $(BOOTLOADER) $(KERNEL)
	dd if=$(BOOTSECTOR) of=$@
	dd if=$(BOOTLOADER) of=$@ seek=1 bs=512
	dd if=$(KERNEL) of=$@ seek=3 bs=512

# build bootsector
$(BOOTSECTOR): $(SRCDIR)/bootsector.asm
	nasm -f bin $< -o $@

# assemble bootloader files
$(BUILDDIR)/bootloader/%.o: $(SRCDIR)/bootloader/%.asm
	nasm -f elf $< -o $@ -I $(SRCDIR)/bootloader/

# link bootloader
$(BOOTLOADER): $(BOOTLOADER_OBJ_FILES)
	i686-elf-gcc -T $(LOADER_LINKER_SCRIPT) -o $(BOOTLOADER) $(LINK_FLAGS) $^

# compile C parts of kernel
$(BUILDDIR)/kernel/c/%.o: $(SRCDIR)/kernel/%.c
	i686-elf-gcc -c $< -o $@ $(CFLAGS)

# assemble asm parts of kernel
$(BUILDDIR)/kernel/asm/%.o: $(SRCDIR)/kernel/%.asm
	nasm -f elf $< -o $@ -i $(SRCDIR)/kernel

# link kernel
$(KERNEL): $(C_OBJ_FILES) $(ASM_OBJ_FILES)
	i686-elf-gcc -T $(KERNEL_LINKER_SCRIPT) -o $(KERNEL_ELF) $^ $(LINK_FLAGS) $(KERNEL_LINK_FLAGS)
	objcopy --only-keep-debug $(KERNEL_ELF) $(KERNEL_SYMBOLS)
	objcopy -O binary $(KERNEL_ELF) $(KERNEL)

clean:
	rm -rf $(BUILDDIR)
	-rm $(IMGDIR)/*
	mkdir -p $(BUILDDIR)/kernel/c $(BUILDDIR)/kernel/asm $(BUILDDIR)/bootloader $(IMGDIR)