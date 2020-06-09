; bootloader.asm: EggOS bootloader

; Start environment: Loaded at 0x0000:0x7E00 (right above bootsector)
; Segment registers zeroed, valid stack exists
; Disk number on stack

BITS 16
ORG 0x7E00

; Defines
MEMORY_MAP_OFFSET equ 0x8600        ; Offset at which the memory map will be loated
NUM_MAP_ENTRIES_OFFSET equ 0x8700   ; Offset at which the number of memory map entries will be located

KERNEL_SIZE_SECTORS equ 21
KERNEL_LOAD_OFFSET equ 0x9000
KERNEL_LOAD_SEGMENT equ 0x0000
KERNEL_START_SECTOR equ 4

; Entry point
start:

    ; Save disk number that was pushed by bootsector
    mov [diskNumber], dx

    ; Print a quick welcome message :)
    mov si, welcomeMessage
    call print

    ; Get memory map
    call doE820
    cmp ax, 0
    jne .errorE820

    ; Enable A20 line
    call enableA20
    cmp ax, 1
    jne .errorA20

    ; Load kernel
    call loadKernel
    cmp ax, 0
    jne .errorLoad

    ; Jump to kernel
    call startKernel

    ; Hang just in case (execution should never get here though)
    jmp hang

.errorE820:
    mov si, errorE820Str
    call print
    jmp hang

.errorA20:
    mov si, errorA20Str
    call print
    jmp hang

.errorLoad:
    ;mov si, errorLoadStr
    ;call print
    jmp hang

; Print a string via BIOS interrupts
print:
    ; AH = function number (0x0E = print char)
    ; Also clear direction flag so string is loaded correctly
    mov ah, 0x0E
    cld
.loop:
    ; Load next character in string and print via BIOS
    lodsb
    cmp al, 0   ; Exit if null (string is done)
    je .done
    int 0x10
    jmp .loop
.done:
    ret

; Hang: halt system indefinitely
hang:
    cli
    hlt
    jmp hang

; Get memory map via BIOS interrupts
; Returns error code if not successful, 0 if success
; BP will contain number of entries read
;
; ERROR CODES:
; 1 = carry flag was set 
; 2 = E820 not supported 
; 3 = the returned list was only 1 entry long
doE820:

    ; Set up for interrupt
    mov eax, 0xE820                 ; EAX = function code (0xE820 = get full memory map)
    mov ebx, 0                      ; EBX = "continuation value" (0 for first call)
    mov di, MEMORY_MAP_OFFSET       ; ES:DI = location of buffer to be filled in
    mov ecx, 24                     ; Size of the buffer (24 since we want a full entry)
    mov edx, 0x534D4150             ; Magic number ('SMAP')

    ; Force entry to be a valid "ACPI" entry
    ; If BIOS doesn't fill the acpi field of the memory map entry, it will still be 1
    mov dword [es:di + 20], 1

    ; Return number of memory map entries in BP
    xor bp, bp

    ; Do the interrupt
    int 0x15
    jc .errorCarry

    ; Reload EDX since on some BIOSes it gets trashed.
    ; If E820 is supported, EAX will have 'SMAP' in it.
    mov edx, 0x534D4150
    cmp eax, edx
    jne .errorNotSupported

    ; If EBX = 0, the last entry has already been reached
    ; 1 entry long list is worthless, assume something is wrong.
    cmp ebx, 0
    je .errorBadList

    ; Do further tests on the entry
    jmp .check

.loop:

    ; ECX, EAX are overwritten by interrupt
    ; Set up
    mov eax, 0xE820
    mov ecx, 24
    mov dword [es:di + 20], 1

    ; Do interrupt again
    int 0x15
    jc .errorCarry

    ; Set up EDX again since it gets trashed on some BIOSes
    mov edx, 0x534D4150

.check:
    
    ; ECX = size of entry returned
    ; if 0, skip
    jcxz .loopCond

    ; Check if it's an ACPi entry. If not,
    cmp cl, 20
    jbe .check2

    ; Check if the "ignore" bit is set.
    test byte [es:di + 20], 1
    je .loopCond

.check2:

    ; If length is zero, skip
    mov ecx, [es:di + 8]
    or ecx, [es:di + 12]
    jz .loopCond

    ; Otherwise, it's a valid entry.
    ; Advance pointer and increase number of entries
    inc bp
    add di, 24

; Check if the loop should run again
.loopCond:

    ; If EBX (continuation) is zero, we're done. 
    test ebx, ebx
    jne .loop

; Clean up and return
.done:

    ; Return 0 for success
    ; Write number of memory map entries to a fixed offset so it can be read by the kernel
    mov ax, 0
    mov [NUM_MAP_ENTRIES_OFFSET], bp
    clc
    ret

; Called when CF is set - indicates error while calling interrupt.
.errorCarry:
    mov ax, 1
    ret

; Called when EAX != 'SMAP' after calling interrupt
; This indicates that E820 isn't supported
.errorNotSupported:
    mov ax, 2
    ret

; Called when E820 returns a 1-long list
.errorBadList:
    mov ax, 3
    ret

; Enable A20 line
enableA20:

    ; Check if A20 is enabled already.
    call checkA20
    cmp ax, 1
    je .done

    ; Try to use the BIOS to enable the A20 line.
    call enableA20_BIOS
    call checkA20
    cmp ax, 1
    je .done

    call enableA20_KBC
    call checkA20
    cmp ax, 1
    je .done

    call enableA20_fast
    call checkA20

.done:
    ret

; Test if the A20 line is enabled.
; Returns 1 in AX if the A20 line is enabled, and 0 if it isn't.
checkA20:

    pushf
    push ds
    push es
    push di
    push si

    cli

    ; Set ES to 0
    xor ax, ax
    mov es, ax

    ; Set DS to 0xFFFF
    not ax
    mov ds, ax

    sti

    ; Set up DI, SI
    mov di, 0x0500
    mov si, 0x0510

    ; Save values in addresses to be tested
    mov al, byte [es:di]
    push ax
    mov al, byte [ds:si]
    push ax

    ; Test if segments wrap around
    mov byte [es:di], 0x00
    mov byte [ds:si], 0xFF

    cmp byte [es:di], 0xFF

    ; Restore values in addresses that were tested
    pop ax
    mov byte [ds:si], al
    pop ax
    mov byte [es:di], al

    ; Check if the test earlier passed
    mov ax, 0
    je .cleanup
    
    mov ax, 1

.cleanup:
    pop si
    pop di
    pop es
    pop ds
    popf

    ret

; Try to enable the A20 line via the BIOS
enableA20_BIOS:

    ; AX = function number (0x2401 = enable A20 line)
    mov ax, 0x2401
    int 0x15

    ret

; Try to enable the A20 line via the keyboard controller
enableA20_KBC:

    cli

    ; Disable 1st PS/2 port
    call .waitWrite
    mov al, 0xAD
    out 0x64, al

    ; Read controller output port (1) - send command
    call .waitWrite
    mov al, 0xD0
    out 0x64, al

    ; Read controller output port (2) - read controller output and push to stack
    call .waitRead
    in al, 0x60
    push eax

    ; Write to controller output port (1) - send command
    call .waitWrite
    mov al, 0xD1
    out 0x64, al

    ; Write to controller output port (2) - write byte
    ; Set the A20 bit in the controller output byte first
    call .waitWrite
    pop eax
    or al, 2
    out 0x60, al

    ; Re-enable first PS/2 port
    call .waitWrite
    mov al, 0xAE
    out 0x64, al

    ; Wait
    call .waitWrite

    sti
    ret

.waitWrite:
    in al, 0x64
    test al, 2
    jnz .waitWrite
    ret

.waitRead:
    in al, 0x64
    test al, 1
    jz .waitRead
    ret

; Try enabling the A20 line through the "fast" method
enableA20_fast:

    ; Read value in port 92. If bit 2 (A20) is already set, skip.
    in al, 0x92
    test al, 2
    jnz .done

    ; Otherwise, set bit and ensure bit 0 (fast reset) is not enabled
    or al, 2
    and al, 0xFE
    out 0x92, al

.done:
    ret

; Load the kernel.
loadKernel:

    ; Make sure BIOS INT 0x13 extensions exist. If not, return error.
    call checkBIOSExt
    cmp ax, 1
    jne .error

    ; Read sectors from disk
    mov ah, 0x42            ; AH = function number (0x42 = extended read)
    mov dx, [diskNumber]       ; DL = disk number
    mov si, .DAP            ; DS:SI = pointer to DAP
    int 0x13
    jc .error
    mov ax, 0
    ret

.DAP:
    db 0x10                 ; Size of DAP (16)
    db 0                    ; Must be zero
    dw KERNEL_SIZE_SECTORS  ; Number of sectors
    dw KERNEL_LOAD_OFFSET   ; Offset at which the kernel will be loaded
    dw KERNEL_LOAD_SEGMENT  ; Sector at which the kernel will be loaded
    dq KERNEL_START_SECTOR  ; Sector at which the kernel starts

.error:
    mov ax, 1
    ret

; Check if BIOS INT 0x13 extensions are available
checkBIOSExt:
    mov ah, 0x41            ; AH = function number
    mov dx, [diskNumber]       ; DL = disk number
    mov bx, 0x55AA          ; BX = magic number
    int 0x13
    mov ax, 0
    jc .done
    mov ax, 1
.done:
    ret

; Start the kernel
startKernel:

    cli

    ; Set up GDT first. This is important.
    lgdt [GDTPointer]

    ; Set PM bit in CR0
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax

    ; Load segment registers
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; Jump to kernel
    jmp CODE_SEG:kernelStub

    ret

; GDT structures
GDTStart:

; First entry in the GDT must be null and is ignored.
GDTNullEntry:
    dq 0x0

; Limit=0xFFFFF [x4096 = 0xFFFFFFFF (4G)]
; Base=0x00000000
; Access: Present=1, RingLevel=0, Executable=1, Direction=0, ReadWrite=1, Accessed=0
; Flags: Granularity=1, Size=1, LimitHigh=1111 (0xF)
GDTCodeEntry:
    dw 0xFFFF
    dw 0x0
    db 0x0
    db 10011010b
    db 11001111b
    db 0x0

; Identical to GDT data entry, except access is different
GDTDataEntry:
    dw 0xFFFF
    dw 0x0
    db 0x0
    db 10010010b
    db 11001111b
    db 0x0

; End of the GDT
GDTEnd:

; Structure that points to the GDT
GDTPointer:
    dw GDTEnd - GDTStart
    dd GDTStart

CODE_SEG equ GDTCodeEntry - GDTStart
DATA_SEG equ GDTDataEntry - GDTStart

; Strings
welcomeMessage db "eggloader started!",13,10,0
errorE820Str db "fatal: couldn't get memory map",13,10,0
errorA20Str db "fatal: couldn't enable A20 line",13,10,0
errorLoadStr db "fatal: couldn't load kernel",13,10,0

; Data
diskNumber db 0             ; Disk number that the OS was booted from

; Pad with zeroes.
TIMES 1024-($-$$) db 0

;------------------------------------------------------------------------------;

; Stub that copies protected-mode kernel to 1M and jumps to it

BITS 32
kernelStub:

    ; Copy kernel to 1M
    xor eax, eax
    mov esi, 0x100000

.loop:
    cmp eax, KERNEL_SIZE_SECTORS * 512
    je .done
    mov ebx, [eax + KERNEL_LOAD_OFFSET]
    mov [esi], ebx
    add esi, 4
    add eax, 4
    jmp .loop
.done:
    jmp 0x100000

.hang:
    cli
    hlt
    jmp .hang