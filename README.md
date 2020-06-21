# egg-kernel
My attempt at writing a small kernel.

## TODO List
* **Recently Completed:**
    * Physical memory allocator
        * Is this robust? Tests necessary!
        * Implement better system
            * Bitmap (current): O(1) set state, O(N) find free
            * Buddy system (Linux-style): purpotedly better?
            
* **Up next:**
    * General paging stuff
        * Figure out how paging bits in PDE, PTE actually affect access
        * (maybe) higher half kernel?
    * Kernel heap
    * Preemptive multitasking
        * How many TSS's necessary?
        * Context switches
    * Usermode
        * Extra GDT's
        * Don't kill the entire system when a usermode exception happens
        * System calls
    * Actual filesystem support
        * Which?
            * echFS: easy, FUSE support, but may be slow
            * FAT: weird, many legacy features
            * exFAT: may be good, investigate
            * NTFS: not many docs available
            * ext2: supposedly difficult, slippery slope (egg kernel may just become another \*NIX!)
            * CUSTOM FILESYSTEM
                * very hard
                * very little support
                * difficult to use
                * BUT, high educational value
