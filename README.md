# egg-kernel
egg-kernel is an x86 kernel written in C.

# how to build?
egg-kernel is built using GNU make and these tools:

* `dd`
* `nasm`
* i686-elf-gcc

You'll probably need to build the cross compiler (i686-elf-gcc) from source. The [OSDev wiki](https://wiki.osdev.org/GCC_Cross-Compiler) has a great guide on this.

# how to run?
For the sake of simplicity, egg-kernel boots using a master boot record, so it needs to be run on an emulator or machine that supports this method of booting.

There are some BIOSes which have certain unwritten expectations about an OS's bootsector, ones which egg-kernel's probably doesn't fulfill. On the off chance that you encounter one.

Running egg-kernel in an emulator is easy. For QEMU:

```
qemu-system-i386 -hda disk.hdd
```

# what does it look like?

Not much. egg-kernel is extremely early in development.

![image of OS](picture.png)
