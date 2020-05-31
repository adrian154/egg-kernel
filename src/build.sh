# Clean up builds first
./clean.sh

# Assemble bootsector
nasm -f bin ./bootloader/bootsector.asm -o ../build/bootsector.bin

# Write binaries to disk image
dd if=../build/bootsector.bin of=../images/disk.img