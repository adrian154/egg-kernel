; IRQ handler code

%include "constants.asm"

; Here, 16 IRQ handlers are defined, which jump to a common stub that passes the IRQ to the kernel.
; C functions can't directly be used as interrupt handlers. The compiler is oblivious to the environment and tends to annihilate the stack.
; (Technically, you can use GCC's interrupt attribute, but that runs the risk of unleashing dark spirits upon the world.)
; Each IRQ has its own subroutine because it's the easiest way to tell *which* IRQ the code is being executed for. We could ask the PIC, but it complicates things slightly. 

EXTERN mainIRQHandler
GLOBAL irq0
GLOBAL irq1
GLOBAL irq2
GLOBAL irq3
GLOBAL irq4
GLOBAL irq5
GLOBAL irq6
GLOBAL irq7
GLOBAL irq8
GLOBAL irq9
GLOBAL irq10
GLOBAL irq11
GLOBAL irq12
GLOBAL irq13
GLOBAL irq14
GLOBAL irq15

; IRQ handlers
irq0:
    push DWORD 0
    jmp commonIRQHandler
irq1:
    push DWORD 1
    jmp commonIRQHandler
irq2:
    push DWORD 2
    jmp commonIRQHandler
irq3:
    push DWORD 3
    jmp commonIRQHandler
irq4:
    push DWORD 4
    jmp commonIRQHandler
irq5:
    push DWORD 5
    jmp commonIRQHandler
irq6:
    push DWORD 6
    jmp commonIRQHandler
irq7:
    push DWORD 7
    jmp commonIRQHandler
irq8:
    push DWORD 8
    jmp commonIRQHandler
irq9:
    push DWORD 9
    jmp commonIRQHandler
irq10:
    push DWORD 10
    jmp commonIRQHandler
irq11:
    push DWORD 11
    jmp commonIRQHandler
irq12:
    push DWORD 12
    jmp commonIRQHandler
irq13:
    push DWORD 13
    jmp commonIRQHandler
irq14:
    push DWORD 14
    jmp commonIRQHandler
irq15:
    push DWORD 15
    jmp commonIRQHandler

commonIRQHandler:

    ; save registers
    pusha
    push ds
    push es
    push fs
    push gs

    ; load ring0 segments
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; pass stack pointer to mainIRQHandler
    mov eax, esp
    push eax

    ; ??? (still don't get this part)
    mov eax, mainIRQHandler
    call eax

    ; restore registers
    pop eax
    pop gs
    pop fs
    pop es
    pop ds
    popa

    ; get rid of the pushed IRQ#
    add esp, 4
    
    ; let CPU restore the remaining registers
    iret