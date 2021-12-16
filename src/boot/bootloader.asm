; bootloader.asm: set up the environment and load the CPU
; * turn on A20 line
; * install a simple GDT
; * enter protected mode

; The bootsector loads the booatloader at 0x7E00, right above the bootsector.
; The segment registers have been setup as well as a small stack.
; The disk number is stored on the stack by the bootsector.

BITS 16
ORG 0x7E00

KERNEL_SIZE_SECTORS equ 57          ; Size of the kernel in sectors
KERNEL_LOAD_OFFSET equ 0x8400       ; Segment-offset where the kernel will be loaded in memory
KERNEL_LOAD_SEGMENT equ 0x0000      ; (...)
KERNEL_START_SECTOR equ 3           ; First sector of kernel on disk

start:

    ; save disk number that was pushed by bootsector
    mov [envdata_disknumber], dx

    ; Print a quick welcome message :)
    mov si, welcome_message
    call print

    ; grab memory map
    call do_e820
    test ax, ax
    jne .error_e820

    ; enable A20 line
    call enable_A20
    test ax, ax
    jne .error_A20

    ; load kernel
    call load_kernel
    test ax, ax
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
    jne .error

    ; read kernel off disk
    mov ah, 0x42                    ; AH = function number (0x42 = extended read)
    mov dx, [envdata_disknumber]    ; DL = disk number
    mov si, .DAP                    ; DS:SI = pointer to DAP
    int 0x13
    jc .error
    mov ax, 0
    ret

; structure that tells the BIOS what and where to read
.DAP:
    db 0x10                 ; size of DAP (16)
    db 0                    ; zero field
    dw KERNEL_SIZE_SECTORS
    dw KERNEL_LOAD_OFFSET
    dw KERNEL_LOAD_SEGMENT 
    dq KERNEL_START_SECTOR 

.error:
    mov ax, 1
    ret

; Check if BIOS INT 0x13 extensions are available
check_int13h_extensions:
    mov ah, 0x41                    ; INT 0x13 AH=0x41: check if extensions are available
    mov dx, [envdata_disk_number]   ; DL = disk number
    mov bx, 0x55AA                  ; BX = magic number
    int 0x13
    mov ax, 0
    jc .done
    mov ax, 1
.done:
    ret

; make transition to protected mode and load the kernel
start_kernel:

    cli

    ; set up a basic GDT
    lgdt [GDT_pointer]

    ; set protected mode bit in CR0
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax

    ; load the new segments (in protected mode segment registers store descriptors)
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; load CS by doing a far jump
    jmp CODE_SEG:pmode_stub

; GDT 
GDT_start:

; first entry in the GDT must be null and is ignored
GDT_null_entry:
    dq 0x0

; limit=0xFFFFF [x4096 = 0xFFFFFFFF (4G)]
; base=0x00000000
; access: Present=1, RingLevel=0, Executable=1, Direction=0, ReadWrite=1, Accessed=0
; flags: Granularity=1, Size=1, LimitHigh=1111 (0xF)
GDT_code_segment:
    dw 0xFFFF
    dw 0x0
    db 0x0
    db 10011010b
    db 11001111b
    db 0x0

; Identical to GDT data entry, except `access` is different
GDT_data_segment:
    dw 0xFFFF
    dw 0x0
    db 0x0
    db 10010010b
    db 11001111b
    db 0x0

; End of the GDT
GDT_end:

; GDT descriptor, which tells the CPU about how to load our table
GDT_pointer:
    dw GDT_end - GDT_start 
    dd GDT_start

CODE_SEG equ GDT_code_segment - GDT_start
DATA_SEG equ GDT_code_segment - GDT_start

;------------------------------------------------------------------------------;
; Stub that copies protected-mode kernel to 1M and jumps to it

BITS 32
pmode_stub:

    ; copy kernel
    xor eax, eax
    mov esi, 0x100000

; FIXME: this code ignores KERNEL_LOAD_SEGMENT
.loop:
    cmp eax, KERNEL_SIZE_SECTORS * 512
    je .done
    mov ebx, [eax + KERNEL_LOAD_OFFSET]
    mov [esi], ebx
    add esi, 4
    add eax, 4
    jmp .loop
.done:

    ; pass envdata to start() and jump
    push DWORD envdata_struct
    jmp 0x100000

.hang:
    cli
    hlt
    jmp .hang

; messages
welcome_message db "eggloader started!",13,10,0
e820_error_message db "fatal: couldn't get memory map",13,10,0
a20_error_message db "fatal: couldn't enable A20 line",13,10,0
load_error_message db "fatal: couldn't load kernel",13,10,0

; struct
environment_data:
    envdata_disknumber db 0
    envdata_mmap_entry_count dd 0
    envdata_mmap_pointer dd 0

GLOBAL envdata_struct
GLOBAL envdata_disknumber
GLOBAL envdata_mmap_entry_count
GLOBAL envdata_mmap_pointer