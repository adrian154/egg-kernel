MMAP_OFFSET equ 0x8200          ; offset at which the memory map will be stored
E820_MAGIC equ 0x534D4150       ; magic number used for E820 calls ('SMAP')
E820_ENTRY_SIZE equ 24          ; size of a full memory map entry

EXTERN envdata_mmap_entry_count
EXTERN envdata_mmap_pointer
GLOBAL do_e820

; Get memory map via BIOS interrupts
; RETURN (AX): 0 on success, 1 if something went wrong
do_e820:

    mov ebx, 0               ; ebx = 'continuation value' (0 for the first call)
    mov di, MMAP_OFFSET      ; memory map entries are written to ES:DI

    ; Clear BP (number of entries)
    xor bp, bp

.loop:

    ; set up for interrupt
    mov eax, 0xE820                 ; INT 0x15 AX = 0xE820: ask the BIOS for a system address map
    mov ecx, E820_ENTRY_SIZE        ; the BIOS doesn't really care about the actual size of the buffer; pass the maximum size of an entry to let it know that it can write the acpi field
    mov edx, E820_MAGIC             ; EDX is a magic number

    ; set the acpi field of the memory map entry to 1 in case the BIOS doesn't set it
    mov dword [es:di + 20], 1

    ; do interrupt
    int 0x15
    jc .error_carry

    ; if E820 is supported, EAX will have 'SMAP' in it.
    cmp eax, E820_MAGIC
    jne .error_not_supported
    
    ; ECX = size of the returned entry
    ; skip empty entries
    jcxz .continue

    ; check if the ignore bit is set (???)
    test byte [es:di + 20], 1
    je .continue

    ; advance pointer and increase number of entries
    inc bp
    add di, E820_ENTRY_SIZE

.continue:

    ; if ebx (continuation) is zero there are no more memory map entries to read 
    test ebx, ebx
    jne .loop

; clean up and return
.done:

    ; return 0 for success
    ; write number of memory map entries to a fixed offset so it can be read by the kernel
    mov ax, 0
    mov [envdata_mmap_entry_count], bp
    mov [envdata_mmap_pointer], DWORD MMAP_OFFSET
    ret

; called when CF is set - indicates error while calling interrupt.
.error:
    mov ax, 1
    ret