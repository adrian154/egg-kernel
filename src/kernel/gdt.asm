; GDT code.

%include "constants.asm"

; Global symbols so we can acccess the GDT pointer
GLOBAL installGDT
EXTERN GDTPointer

; Makes CPU aware of our GDT
installGDT:
    lgdt [GDTPointer]
    
    ; Set up segment registers
    ; (In pmode, segment registers store selectors)
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp CODE_SEG:.cleanup
.cleanup:
    ret