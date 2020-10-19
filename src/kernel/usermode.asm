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

    ; IRET pops flags and some selectors n shit
    ; "Trick" the processor by fiddling with the stack
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