BITS 16
SECTION .text

GLOBAL enable_A20

; the number of hoops you (potentially) have to jump through to enable the A20 line gives you a good idea of how painful it is to develop for a system with 45 years of legacy baggage
; returns 0 (failure) or 1 (success) in AX
enable_A20:

    ; some BIOSes enable A20 for you
    call check_A20
    cmp ax, 1
    je .done

    ; try to use the BIOS to enable the A20 line
    call enableA20_BIOS
    call check_A20
    cmp ax, 1
    je .done

    call enableA20_KBC
    call check_A20
    cmp ax, 1
    je .done

    call enableA20_fast
    call check_A20 ; no need to check the return value of check_A20; if it failed 0 will already be in AX

.done:
    ret

; check if memory accesses above 1M wrap around by looking at the bootsector signature
; one caveat is that if for some reason the A20 line is enabled but the value 0xAA55 has also been written to the test location this code will return an incorrect result
; returns 0 (failure) or 1 (success) in AX
check_A20:

    ; save segments to prevent bad things from happening
    pushf
    push es
    push di
    cli

    ; ES:DI = 0xFFFF:0x7e0e = 0x107dfe if A20 is on, 0x7dfe if not
    mov ax, 0xFFFF
    mov es, ax
    mov di, 0x7e0e

    ; if equal, A20 isn't enabled
    cmp word [es:di], 0xAA55
    mov ax, 0
    je .cleanup
    
    mov ax, 1

.cleanup:
    pop di
    pop es
    popf
    ret

; try to enable the A20 line via the BIOS
enableA20_BIOS:

    ; INT 0x15 AX = 0x2401: enable A20 line
    mov ax, 0x2401
    int 0x15
    ret

; Try to enable the A20 line via the keyboard controller
enableA20_KBC:

    pushf
    cli

    call .wait_write
    mov al, 0xAD        ; 0xAD = disable first PS/2 port
    out 0x64, al

    call .wait_write
    mov al, 0xD0        ; 0xD0 = read controller output port
    out 0x64, al

    call .wait_read     ; wait for the value of the controller output port to be available
    in al, 0x60

    ; bit 1 of the controller output port's value indicates whether the A20 line is on
    or al, 2
    push ax             ; save value on stack

    call .wait_write
    mov al, 0xD1        ; 0xD1 = write to controller output port
    out 0x64, al

    call .wait_write
    pop ax  
    out 0x60, al        ; actually write controller output port value

    call .wait_write
    mov al, 0xAE        ; 0xAE = enable first PS/2 port
    out 0x64, al

    ; wait for the kbc to catch up 
    call .wait_write

    popf
    ret

; bit 0 of the status register (0x64) indicates whether data can be written
.wait_write:
    in al, 0x64
    test al, 2
    jnz .wait_write
    ret

; bit 1 of the status register (0x64) indicates whether data can be read
.wait_read:
    in al, 0x64
    test al, 1
    jz .wait_read
    ret

; Try enabling the A20 line through the "fast" method
enableA20_fast:

    ; read value in port 0x92
    in al, 0x92

    ; set bit 1 (A20)
    ; clear bit 0 (used for fast reset)
    or al, 2
    and al, 0xFE
    out 0x92, al

.done:
    ret