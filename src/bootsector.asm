; bootsector.asm: EggOS bootsector
; The bootsector simply loads the next stage of the OS, the bootloader

; The CPU always starts execution from a physical address of 0x7C00
; However, the actual values of CS/IP are not guaranteed.
; This can be fixed by doing a far jump to set the values of both registers.
; DL will have the disk number

BITS 16
ORG 0x7C00

; How large the bootloader is on disk
SECTORS_TO_LOAD equ 3

; Set CS/IP by doing a far jump
jmp 0x0000:start

start:

    ; Prevent interrupts while the stack and segment registers are being set up
    ; If an interrupt occurred, things could get messy
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

    ; Save disk number on stack
    push dx

    ; Switch VGA mode to text
    ; This is necessary on some devices that do not start in text mode
    call setTextmode

    ; Load stage 2
    call loadStage2
    cmp ah, 0
    jne .errorLoading

    ; Jump to bootloader
    jmp 0x0000:0x7E00

    ; Hang (execution should never get here though)
    jmp hang

; Called if stage 2 can't be loaded
.errorLoading:
    mov eax, 0xDEADDEAD ; for debugging purposes
    jmp hang

; Enable textmode
; Set VGA mode to 3 (text) via BIOS interrupt 0x10 AH=0
setTextmode:
    xor ah, ah
    mov al, 0x03
    int 0x10
    ret

; Load stage 2 of the bootloader
; Use BIOS interrupt 0x13 to load sectors
loadStage2:
    ; DL still contains the drive number
    mov ah, 0x42            ; AH = function number (0x42 = "extended read")
    mov si, DAP             ; DS:SI = pointer to DAP (structure containing data for interrupt)
    int 0x13                ; Read data into buffer
    ret

DAP:
    db 0x10                 ; Size of the DAP (always 16)
    db 0x00                 ; Must be zero
    dw SECTORS_TO_LOAD      ; Number of sectors
    dw 0x7E00               ; Buffer offset
    dw 0x0000               ; Buffer segment
    dq 1                    ; LBA sector to start at (bootsector = first, 0)

; Halt system forever
hang:
    cli
    hlt
    jmp hang

; Pad sectors with zero, leave space for magic number
TIMES 510-($-$$) db 0
dw 0xAA55