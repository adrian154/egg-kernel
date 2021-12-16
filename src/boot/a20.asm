; the number of hoops you (potentially) have to jump through to enable the A20 line is exemplary of how painful it is to develop for a system with 45 years of legacy baggage
; returns 0 (failure) or 1 (success) in AX
enableA20:

    ; some BIOSes enable A20 for you
    call check_A20
    cmp ax, 1
    je .done

    ; try to use the BIOS to enable the A20 line
    call enable_A20_BIOS
    call check_A20
    cmp ax, 1
    je .done

    call enableA20_KBC
    call check_A20
    cmp ax, 1
    je .done

    call enableA20_fast
    call check_A20 ; no need to check the return value of check_A20; if it failed 0 is already in AX

.done:
    ret

; test if the A20 line is enabled
; returns 0 (failure) or 1 (success) in AX
check_A20:

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