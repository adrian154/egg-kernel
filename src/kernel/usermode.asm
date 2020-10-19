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

    ; IRET pops flags and some selectors n shit off the stack
    ; "Trick" the processor by pushing some extra things
    ; I'll bet it never saw that one coming!
    ; it's late
    mov eax, esp
    push USER_DATA_SEG
    push eax
    pushf
    push USER_CODE_SEG
    push usermodeFunc
    
    ; the trickery goes on here
    iret

; try a privileged insn, this should cause a GPF
usermodeFunc:
    cli