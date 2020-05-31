# Clean up builds first
./clean.sh

# Assemble bootsector and bootloader
nasm -f bin ./bootloader/bootsector.asm -o ../build/bootsector.bin
nasm -f bin ./bootloader/bootloader.asm -o ../build/bootloader.bin

# Write binaries to disk image
dd if=../build/bootsector.bin of=../images/disk.img
dd if=../build/bootloader.bin of=../images/disk.img seek=1 bs=512