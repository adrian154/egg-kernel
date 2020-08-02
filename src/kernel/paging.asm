GLOBAL loadPageDirectory
GLOBAL enablePaging
GLOBAL invalidateMapping
GLOBAL flushAllPages

; C prototype: void loadPageDirectory(uint32_t *pageDirectory)
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

; C prototype: void enablePaging()
; Enable paging by setting bit in CR0
enablePaging:

    ; Set PG bit (bit 31)
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax
    ret

; C prototype: void invalidateMapping(void *pageMapping)
; Invalidate TLB entry for a logical address
; MUST be used when updating page mappings
invalidateMapping:

    ; This instruction only works on 486+ ISAs
    ; However, the kernel is not intended to be i386-compatible
    mov LONG eax, [esp + 4]
    invlpg [eax]
    ret

; C prototype: void flushAllPages()
; Invalidates all TLB entries by reloading cr3
; Not very performant, and unnecessary if your change already loads CR3 (e.g. loading a page directory)
flushAllPages:

    mov eax, cr3
    mov cr3, eax
    ret
