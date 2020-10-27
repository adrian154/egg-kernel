export OBJECT_FILES="../build/init.o ../build/main.o ../build/ioport.o ../build/terminal.o ../build/gdt.o ../build/gdt_asm.o ../build/idt_asm.o ../build/idt.o ../build/exception_asm.o ../build/exception.o ../build/pic.o ../build/irq.o ../build/irq_asm.o ../build/physalloc.o ../build/paging.o ../build/paging_asm.o ../build/kernalloc.o ../build/tss.o ../build/usermode.o"
export CFLAGS="-ffreestanding -Wall -Wextra -std=gnu11 -O2"
export LDFLAGS="-ffreestanding -nostdlib -lgcc -O2 -Xlinker -Map=../debug/kernel.map"

mkdir -p ../build
mkdir -p ../debug

# Assemble bootsector and bootloader
nasm -f bin ./boot/bootsector.asm -o ../build/bootsector.bin
nasm -f bin ./boot/bootloader.asm -o ../build/bootloader.bin

# Assemble protected-mode parts of the kernel
nasm -f elf ./kernel/init.asm -o ../build/init.o
nasm -f elf ./kernel/ioport.asm -o ../build/ioport.o
nasm -f elf ./kernel/gdt.asm -o ../build/gdt_asm.o -i./kernel/
nasm -f elf ./kernel/idt.asm -o ../build/idt_asm.o
nasm -f elf ./kernel/exception.asm -o ../build/exception_asm.o -i./kernel/
nasm -f elf ./kernel/irq.asm -o ../build/irq_asm.o -i./kernel/
nasm -f elf ./kernel/paging.asm -o ../build/paging_asm.o -i./kernel/
nasm -f elf ./kernel/tss.asm -o ../build/tss.o -i./kernel/
nasm -f elf ./kernel/usermode.asm -o ../build/usermode.o -i./kernel/

# Compile C parts of kernel
i686-elf-gcc -c ./kernel/main.c -o ../build/main.o $CFLAGS
i686-elf-gcc -c ./kernel/terminal.c -o ../build/terminal.o $CFLAGS
i686-elf-gcc -c ./kernel/gdt.c -o ../build/gdt.o $CFLAGS
i686-elf-gcc -c ./kernel/idt.c -o ../build/idt.o $CFLAGS
i686-elf-gcc -c ./kernel/exception.c -o ../build/exception.o $CFLAGS
i686-elf-gcc -c ./kernel/pic.c -o ../build/pic.o $CFLAGS
i686-elf-gcc -c ./kernel/irq.c -o ../build/irq.o $CFLAGS
i686-elf-gcc -c ./kernel/physalloc.c -o ../build/physalloc.o $CFLAGS
i686-elf-gcc -c ./kernel/paging.c -o ../build/paging.o $CFLAGS
i686-elf-gcc -c ./kernel/kernalloc.c -o ../build/kernalloc.o $CFLAGS

# Link kernel
i686-elf-gcc -T ./kernel/linker.ld -o ../build/kernel.bin $LDFLAGS $OBJECT_FILES

# Write binaries to disk image
dd if=../build/bootsector.bin of=../images/disk.hdd
dd if=../build/bootloader.bin of=../images/disk.hdd seek=1 bs=512
dd if=../build/kernel.bin of=../images/disk.hdd seek=3 bs=512

# Make a copy of the disk image so it can be viewed while QEMU is running
cp ../images/disk.hdd ../images/disk_copy.hdd
