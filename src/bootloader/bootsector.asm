; bootsector.asm: EggOS bootsector

; Start environment: real mode, physical address = 0x7C00
; No guarantees about actual CS/IP
; DL = disk number

BITS 16
ORG 0x7C00

; Some defines
SECTORS_TO_LOAD equ 1

; Set CS/IP by doing a far jump
jmp 0x0000:start

; Entry point
start:

    ; Set up segments and stack
    cli

    ; Zero all segment registers
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; Stack grows down from physical 0x7BFF, right below bootsector
    mov ss, ax
    mov sp, 0x7BFF

    sti

    ; Save disk number on stack
    push dx

    ; Switch VGA mode to text
    ; This is necessary on some devices that do not start in text mode
    call setTextmode

    ; Load stage 2
    call loadStage2

    ; Hang (execution should never get here though)
    jmp hang

; Enable textmode
; Set VGA mode to 3 (text) via BIOS interrupt 0x10
setTextmode:
    xor ah, ah
    mov al, 0x03
    int 0x10
    ret

; Load stage 2 of the bootloader
; Use BIOS interrupt 0x13 to load sectors
loadStage2:
    ; DL should have the drive index, which is already set for us by the boot sequence
    mov ah, 0x42            ; AH = function number (0x42 = "extended read")
    mov si, DAP             ; DS:SI = pointer to DAP (structure containing data for interrupt)
    int 0x42                ; Read data into buffer

DAP:
    db 0x10                 ; Size of the DAP (always 16)
    db 0x00                 ; Must be zero
    dw SECTORS_TO_LOAD      ; Number of sectors
    dw 0x7E00               ; Buffer offset
    dw 0x0000               ; Buffer segment
    dq 1                    ; LBA sector to start at

; Hang system forever
hang:
    cli
    hlt
    jmp hang

; Pad sectors with zero, leave space for magic number
TIMES 510-($-$$) db 0
dw 0xAA55