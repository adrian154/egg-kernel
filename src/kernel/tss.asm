%include "constants.asm"
GLOBAL installTSS

; The TSS is pointed to by a descriptor in the GDT
; The CPU stores the current TSS descriptor as a selector in the task register, which can be loaded with the `LTR` instruction
installTSS:
    mov ax, TSS_SEG
    ltr ax
    ret