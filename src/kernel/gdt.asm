; GDT code.

; Global symbols so we can acccess the GDT pointer
GLOBAL installGDT
EXTERN GDTPointer

CODE_SEG equ 0x08   ; PL=00 (ring0), Table=0 (GDT), Index=1 (first entry in GDT)
DATA_SEG equ 0x10   ; PL=00 (ring0), Table=0 (GDT), Index=2 (second entry in GDT)

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