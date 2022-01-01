# egg-kernel
egg-kernel is a tiny x86 kernel, started as an educational project to learn about the internals of operating systems.

# how to build?

To build egg-kernel you'll need these tools:

* `dd`
* `nasm`
* `i686-elf-gcc`
* `virtualbox` (optional)

All of these tools are probably contained on your distro's repos except the cross compiler, which you may need to build from source. The [OSDev wiki](https://wiki.osdev.org/GCC_Cross-Compiler) has a great guide on how to do this.

Once you're ready, just run `make` in the root directory. Optionally, you can also do `make vmdk` to produce a VMDK image that can be used with VirtualBox. This requires `virtualbox` to be installed.

# how to run?

To boot the kernel using QEMU, do:

```
qemu-system-i386 -drive file=disk.img,format=raw
```

Trying to boot egg-kernel on real hardware may or may not work since it lacks a proper MBR, though that can easily be fixed. A bigger problem is that many modern BIOSes only support booting via UEFI.

# what does it look like?
![image of OS](picture.png)