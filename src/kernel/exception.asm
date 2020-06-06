; Exception handler code

%include "constants.asm"

; 2nd half of the exception handler, in C
EXTERN exceptionHandler

; Expose some symbols
GLOBAL isr0
GLOBAL isr1
GLOBAL isr2
GLOBAL isr3
GLOBAL isr4
GLOBAL isr5
GLOBAL isr6
GLOBAL isr7
GLOBAL isr8
GLOBAL isr9
GLOBAL isr10
GLOBAL isr11
GLOBAL isr12
GLOBAL isr13
GLOBAL isr14
GLOBAL isr15
GLOBAL isr16
GLOBAL isr17
GLOBAL isr18
GLOBAL isr19
GLOBAL isr20
GLOBAL isr21
GLOBAL isr22
GLOBAL isr23
GLOBAL isr24
GLOBAL isr25
GLOBAL isr26
GLOBAL isr27
GLOBAL isr28
GLOBAL isr29
GLOBAL isr30
GLOBAL isr31

; All exception handlers push EFLAGS, CS, and EIP.
; Some exceptions (17/#AC, 14/#PF, 13/#GP, 12/#SS, 11/#NP, 10/#TS, and 8/#DF) push an error code to the stack
; For exceptions that DON'T push an error code, we push a dummy error code to avoid alignment issues
; We also push the exception number so all exceptions can be directed to one generic handler in C

isr0:
    push DWORD 0
    push DWORD 0
    jmp commonExceptionHandler
isr1:
    push DWORD 0
    push DWORD 1
    jmp commonExceptionHandler
isr2:
    push DWORD 0
    push DWORD 2
    jmp commonExceptionHandler
isr3:
    push DWORD 0
    push DWORD 3
    jmp commonExceptionHandler
isr4:
    push DWORD 0
    push DWORD 4
    jmp commonExceptionHandler
isr5:
    push DWORD 0
    push DWORD 5
    jmp commonExceptionHandler
isr6:
    push DWORD 0
    push DWORD 6
    jmp commonExceptionHandler
isr7:
    push DWORD 0
    push DWORD 7
    jmp commonExceptionHandler
isr8:
    push DWORD 8        ; Interrupt 8 (#DF) has an error code
    jmp commonExceptionHandler
isr9:
    push DWORD 0
    push DWORD 9
    jmp commonExceptionHandler
isr10:
    push DWORD 10       ; Interrupt 10 (#TS) has an error code
    jmp commonExceptionHandler
isr11:
    push DWORD 11       ; Interrupt 11 (#NP) has an error code
    jmp commonExceptionHandler
isr12:
    push DWORD 12       ; Interrupt 12 (#SS) has an error code
    jmp commonExceptionHandler
isr13:
    push DWORD 13       ; Interrupt 13 (#GP) has an error code
    jmp commonExceptionHandler
isr14:
    push DWORD 14       ; Interrupt 14 (#PF) has an error code
    jmp commonExceptionHandler
isr15:
    push DWORD 0
    push DWORD 15
    jmp commonExceptionHandler
isr16:
    push DWORD 0
    push DWORD 16
    jmp commonExceptionHandler
isr17:
    push DWORD 17       ; Interrupt 17 (#AC) has an error code
    jmp commonExceptionHandler
isr18:
    push DWORD 0
    push DWORD 18
    jmp commonExceptionHandler
isr19:
    push DWORD 0
    push DWORD 19
    jmp commonExceptionHandler
isr20:
    push DWORD 0
    push DWORD 20
    jmp commonExceptionHandler
isr21:
    push DWORD 0
    push DWORD 21
    jmp commonExceptionHandler
isr22:
    push DWORD 0
    push DWORD 22
    jmp commonExceptionHandler
isr23:
    push DWORD 0
    push DWORD 23
    jmp commonExceptionHandler
isr24:
    push DWORD 0
    push DWORD 24
    jmp commonExceptionHandler
isr25:
    push DWORD 0
    push DWORD 25
    jmp commonExceptionHandler
isr26:
    push DWORD 0
    push DWORD 26
    jmp commonExceptionHandler
isr27:
    push DWORD 0
    push DWORD 27
    jmp commonExceptionHandler
isr28:
    push DWORD 0
    push DWORD 28
    jmp commonExceptionHandler
isr29:
    push DWORD 0
    push DWORD 29
    jmp commonExceptionHandler
isr30:
    push DWORD 0
    push DWORD 30
    jmp commonExceptionHandler
isr31:
    push DWORD 0
    push DWORD 31
    jmp commonExceptionHandler

commonExceptionHandler:
    iret

commonExceptionHandler2:
    
    pusha

    ; Save SOME segment registers on stack
    ; If the interrupted code was ring0, no stack change happens
    ; If the interrupted code wasn't ring0, SS:ESP is already saved by the CPU
    ; CS is always saved
    push ds
    push es
    push fs
    push gs

    ; Load ring0 data segment selector into segment registers
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; Pass ESP to exceptionHandler() since an ExceptionFrame is stored on stack
    mov eax, esp
    push eax

    ; Not sure why this is necessary, but most code seems to do this...
    mov eax, exceptionHandler
    call eax

    ; Restore state
    pop gs
    pop fs
    pop es
    pop ds
    popa

    ; Get rid of pushed error code, ISR number
    ; (nothing to restore here)
    add esp, 8

    ; IRET tells CPU to pop CS, EIP, EFLAGS, and the user SS/ESP if necessary
    iret