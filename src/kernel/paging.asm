GLOBAL loadPageDirectory
GLOBAL enablePaging

; C prototype: loadPageDirectory(void *pageDirectory)
; Put pointer to page directory in CR3
loadPageDirectory:

    ; Function prologue
    push ebp
    mov ebp, esp

    ; Pop pointer
    mov eax, [esp + 8]
    mov cr3, eax

    ; Epilogue
    mov esp, ebp
    pop ebp
    ret

; Enable paging by setting bit in CR0
enablePaging:

    ; Set PG bit (bit 31)
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax
    ret

