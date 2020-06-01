; init.asm: Sets up environment for OS

; Loaded in protected mode at 1M

BITS 32

; Expose some symbols so linker doesn't throw a fit
GLOBAL start

start:
    mov eax, 0xD34DB33F
.h:
    cli
    hlt
    jmp .h