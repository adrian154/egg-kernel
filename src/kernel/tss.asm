; TSS code
; The TSS itself is created in C (tss.c)

%include "constants.asm"

; Symbols
GLOBAL installTSS

; Make CPU aware of the TSS
; For some godawful reason, on x86 the LTR instruction takes a selector,
; ...so a GDT entry has to be reserved for the TSS.
installTSS:
    mov ax, TSS_SEG
    ltr ax
    ret