; init.asm: Sets up environment for OS

; Loaded in protected mode at 1M

BITS 32

; Expose some symbols so linker doesn't throw a fit
GLOBAL start

; Some symbols
EXTERN cmain
EXTERN kernelPhysicalStart
EXTERN kernelPhysicalEnd

start:

    ; Set up a stack for the kernel
    mov ebp, stack_bottom
    mov esp, stack_top

    push kernelPhysicalEnd
    push kernelPhysicalStart

    ; Call kernel
    call cmain

    ; Halt just in case (but execution should never get here!)
    jmp hang

hang:
    cli
    hlt
    jmp hang

ALIGN 16
stack_bottom:
    resb 4096
stack_top:
