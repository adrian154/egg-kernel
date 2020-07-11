; IRQ handler code

%include "constants.asm"

; The kernel remaps the PIC so that IRQs 0..16 trigger interrupts 32..48
; There are 16 IRQ handlers that jump to a common stub, which sets up the environment for the C IRQ handler
; The C IRQ handling code dispatches another IRQ handler for that specific IRQ (irq.c)

; 2nd half of the IRQ handler, in C
EXTERN mainIRQHandler

; Expose symbols so they can be put into the IDT by C code
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
    push DWORD 32
    jmp commonIRQHandler
irq1:
    push DWORD 33
    jmp commonIRQHandler
irq2:
    push DWORD 34
    jmp commonIRQHandler
irq3:
    push DWORD 35
    jmp commonIRQHandler
irq4:
    push DWORD 36
    jmp commonIRQHandler
irq5:
    push DWORD 37
    jmp commonIRQHandler
irq6:
    push DWORD 38
    jmp commonIRQHandler
irq7:
    push DWORD 39
    jmp commonIRQHandler
irq8:
    push DWORD 40
    jmp commonIRQHandler
irq9:
    push DWORD 41
    jmp commonIRQHandler
irq10:
    push DWORD 42
    jmp commonIRQHandler
irq11:
    push DWORD 43
    jmp commonIRQHandler
irq12:
    push DWORD 44
    jmp commonIRQHandler
irq13:
    push DWORD 45
    jmp commonIRQHandler
irq14:
    push DWORD 46
    jmp commonIRQHandler
irq15:
    push DWORD 47
    jmp commonIRQHandler

commonIRQHandler:

    ; Save registers - see exception.asm
    pusha
    push ds
    push es
    push fs
    push gs

    ; Load ring0 data segment selector
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; Pass ESP to IRQHandler() since we've effectively constructed an IRQFrame on stack
    mov eax, esp
    push eax

    ; ??? (still don't get this part)
    mov eax, mainIRQHandler
    call eax

    ; Restore registers
    pop eax
    pop gs
    pop fs
    pop es
    pop ds
    popa

    ; Get rid of pushed interrupt number
    add esp, 4
    
    ; CPU does the rest of the cleanup :)
    iret