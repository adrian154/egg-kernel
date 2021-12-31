# egg-kernel
egg-kernel is a tiny x86 kernel, started as an educational project to learn about the internals of operating systems.

# how to build?

To build egg-kernel you'll need these tools:

* `dd`
* `nasm`
* `qemu-utils` (for converting flat images to VirtualBox images)
* `i686-elf-gcc`

All of these tools can be obtained through your package manager except the cross compiler, which you probably need to build from source. The [OSDev wiki](https://wiki.osdev.org/GCC_Cross-Compiler) has a great guide on this.

Once you're ready, just run `make` in the root directory.

# how to run?

To boot the kernel using QEMU, do:

```
qemu-system-i386 -drive file=disk.img,format=raw
```

Trying to boot egg-kernel on modern hardware probably won't work since it doesn't define a proper MBR, though that can easily be fixed. A bigger problem is that many modern BIOSes no longer support booting MBRs. 

# what does it look like?
![image of OS](picture.png)