; bootloader.asm: set up the environment and load the CPU
; * turn on A20 line
; * install a simple GDT
; * enter protected mode

; The bootsector loads the booatloader at 0x7E00, right above the bootsector.
; The segment registers have been setup as well as a small stack.
; The disk number is stored on the stack by the bootsector.

%include "constants.asm"

BITS 16
SECTION .text

GLOBAL start
GLOBAL envdata_struct
GLOBAL envdata_disknumber
GLOBAL envdata_mmap_entry_count
GLOBAL envdata_mmap_pointer

EXTERN do_e820
EXTERN enable_A20
EXTERN start_kernel

start:

    ; save disk number that was pushed by bootsector
    mov [envdata_disknumber], dx

    ; Print a quick welcome message :)
    mov si, welcome_message
    call print

    ; grab memory map
    call do_e820
    cmp ax, 1
    jne .error_e820

    ; enable A20 line
    call enable_A20
    cmp ax, 1
    jne .error_A20

    ; load kernel
    call load_kernel
    cmp ax, 1
    jne .error_loading_kernel

    ; start the kernel
    call start_kernel
    jmp hang

; various error handlers
.error_e820:
    mov si, e820_error_message
    jmp .error
.error_A20:
    mov si, a20_error_message
    jmp .error
.error_loading_kernel:
    mov si, load_error_message
    jmp .error
.error:
    call print
    jmp hang

hang:
    cli
    hlt
    jmp hang

; print a string via BIOS interrupts
print:
    mov ah, 0x0E    ; INT 0x10 AH=0x0E: print character (BIOS handles return and scrolling)
    cld             ; the direction of lodsb depends on the direction flag, which is currently in an undefined state
.loop:
    lodsb           ; load next character
    cmp al, 0       ; exit condition
    je .done
    int 0x10        ; do interrupt
    jmp .loop
.done:
    ret

; load the kernel
load_kernel:

    ; check if the LBA extensions for INT 0x13 exist
    ; fail if unavailable because I refuse to work with CHS
    call check_int13h_extensions
    test ax, ax
    jz .error

    ; read kernel off disk
    mov ah, 0x42                    ; AH = function number (0x42 = extended read)
    mov dx, [envdata_disknumber]    ; DL = disk number
    mov si, .DAP                    ; DS:SI = pointer to DAP
    int 0x13
    jc .error

    xor ax, ax
    ret

.error:
    mov ax, 1
    ret

; structure that tells the BIOS what and where to read
.DAP:
    db 0x10                 ; size of DAP (16)
    db 0                    ; zero field
    dw KERNEL_SIZE_SECTORS
    dw KERNEL_LOAD_OFFSET
    dw KERNEL_LOAD_SEGMENT 
    dq KERNEL_START_SECTOR 

; Check if BIOS INT 0x13 extensions are available
; 1 if available, 0 if not
check_int13h_extensions:
    mov ah, 0x41                    ; INT 0x13 AH=0x41: check if extensions are available
    mov dx, [envdata_disknumber]    ; DL = disk number
    mov bx, 0x55AA                  ; BX = magic number
    int 0x13
    mov ax, 0
    jc .done
    mov ax, 1
.done:
    ret

; messages
welcome_message db "eggloader started!",13,10,0
e820_error_message db "fatal: couldn't get memory map",13,10,0
a20_error_message db "fatal: couldn't enable A20 line",13,10,0
load_error_message db "fatal: couldn't load kernel",13,10,0

; struct
envdata_struct:
    envdata_disknumber db 0
    envdata_mmap_entry_count dd 0
    envdata_mmap_pointer dd 0