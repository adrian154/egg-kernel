# Clean up builds first
./clean.sh

# Assemble bootsector and bootloader
nasm -f bin ./bootloader/bootsector.asm -o ../build/bootsector.bin
nasm -f bin ./bootloader/bootloader.asm -o ../build/bootloader.bin

# Assemble protected-mode parts of the kernel
nasm -f elf ./kernel/init.asm -o ../build/init.o

# Link kernel
i686-elf-gcc -T ./kernel/linker.ld -o ../build/kernel.bin -ffreestanding -O2 -nostdlib ../build/init.o 

# Write binaries to disk image
dd if=../build/bootsector.bin of=../images/disk.img
dd if=../build/bootloader.bin of=../images/disk.img seek=1 bs=512
dd if=../build/kernel.bin of=../images/disk.img seek=4 bs=512

# Make a copy of the disk image so it can be viewed while QEMU is running
cp ../images/disk.img ../images/disk_copy.img