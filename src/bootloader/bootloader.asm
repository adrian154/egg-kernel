; bootloader.asm: EggOS bootloader

; Start environment: Loaded at 0x0000:0x7E00 (right above bootsector)
; Segment registers zeroed, valid stack exists
; Disk number on stack

BITS 16
ORG 0x7E00

; Entry point
start:

    ; Put test on screen
    mov ax, 0xB800
    mov es, ax
    mov di, 0x0000
    mov byte [es:di], 0x41
    inc di
    mov byte [es:di], 0xF2

    ; Hang just in case (execution should never get here though)
    jmp hang

; Hang: halt system indefinitely
hang:
    cli
    hlt
    jmp hang