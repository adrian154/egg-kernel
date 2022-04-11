; init.asm: Sets up environment for OS

; Loaded in protected mode at 1M
BITS 32

; Expose some symbols so linker doesn't throw a fit
GLOBAL start

; Symbols defined by the linker that are passed to the kernel
EXTERN cmain
EXTERN kernelPhysicalStart
EXTERN kernelPhysicalEnd

SECTION .start

start:

    ; Pop environmentData (see bootloader.asm) off old stack
    pop eax

    ; Set up a stack for the kernel
    mov ebp, kernelStackBottom
    mov esp, kernelStack

    ; Push parameters to the kernel
    ; EAX has a pointer to the environmentData struct
    push interruptStack
    push kernelPhysicalEnd
    push kernelPhysicalStart
    
    push eax
    
    ; Call kernel
    call cmain

    ; Halt just in case (but execution should never get here!)
    jmp hang

hang:
    cli
    hlt
    jmp hang

SECTION .bss

; reserve two stacks for the kernel
ALIGN 16
kernelStackBottom:
    resb 4096
kernelStack:
    resb 4096
interruptStack: