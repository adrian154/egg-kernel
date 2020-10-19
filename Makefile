# --- Directories
SRCDIR := src
BUILDDIR := build
IMGDIR := img

# produce two copies of output image so it can be viewed with a hex editor during emulation
OUTIMG := $(IMGDIR)/disk.hdd
COPYIMG := $(IMGDIR)/disk_copy.hdd

# --- Flags
CFLAGS := -ffreestanding -Wall -Wextra -std=gnu11 -O2
ASM_BOOT_FLAGS := -f elf

# --- Other filelists
BOOTSECTOR_DEPS := $(BUILDDIR)/boot/bootsector.bin 
BOOTLOADER_DEPS := $(BUILDDIR)/boot/bootloader.bin
KERNEL_DEPS := $(BUILDDIR)/kernel/kernel.bin

# --- Phonies
.PHONY: all clean

# --- Rules
all: $(OUTIMG)
	cp $(OUTIMG) $(COPYIMG)

$(OUTIMG): $(BOOTSECTOR_DEPS) $(BOOTLOADER_DEPS) $(KERNEL_DEPS)
	dd if=$(BOOTSECTOR_DEPS) of=$@
	dd if=$(BOOTLOADER_DEPS) of=$@ seek=1 bs=512
	dd if=$(KERNEL_DEPS) of=$@ seek=3 bs=512

# Generic bootloader rule
$(BUILDDIR)/boot/%.bin: $(SRCDIR)/boot/%.asm
	nasm $(ASM_BOOT_FLAGS) $< -o $@

$(KERNEL_DEPS): 
	# TODO: ...

clean:
	rm -rf $(BUILDDIR)/*
	rm -rf $(IMGDIR)/*