%include "constants.asm"

BITS 16
SECTION .text

EXTERN envdata_struct
GLOBAL start_kernel

; at this point the kernel has already been loaded at a known location in low memory
; make transition to protected mode so we can jump to the kernel
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

; segment offsets
CODE_SEG equ GDT_code_segment - GDT_start
DATA_SEG equ GDT_data_segment - GDT_start

;------------------------------------------------------------------------------;
; Stub that copies protected-mode kernel to 1M and jumps to it

BITS 32
pmode_stub:

    ; eax = index, esi = ptr to destination
    xor eax, eax
    mov esi, 0x100000

.loop:
    cmp eax, KERNEL_SIZE_SECTORS * 512
    je .done
    mov ebx, [eax + (KERNEL_LOAD_OFFSET + KERNEL_LOAD_SEGMENT * 4)]
    mov [esi], ebx
    add esi, 4
    add eax, 4
    jmp .loop
.done:

    ; pass envdata to `start` and jump
    push DWORD envdata_struct
    jmp 0x100000

; stop execution here in case something weird happens
.hang:
    cli
    hlt
    jmp .hang