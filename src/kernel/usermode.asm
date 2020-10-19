; Usermode switch code

%include "constants.asm"

GLOBAL testEnterUsermode

testEnterUsermode:

    ; load segments
    ; SS is handled by IRET
    mov ax, USER_CODE_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; IRET pops CS:EIP, EFLAGS, and SS:ESP
    ; The CPU expects that these don't change so it can restore processor state before the interrupt
    ; However, we're not in an interrupt, so we push these values on to the stack and trick the CPU so that it jumps to a usermode segment
    mov eax, esp        ; Save ESP from before pushing data segment
    push USER_DATA_SEG  ; Push stack segment
    push eax            ; Push saved ESP -> [SS:ESP]
    pushf               ; FLAGS
    push USER_CODE_SEG  ; Return CS
    push usermodeFunc   ; Return EIP -> [CS:EIP]

    ; the trickery goes on here
    iret

; try a privileged insn, this should cause a GPF
usermodeFunc:
    cli