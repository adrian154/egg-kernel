; IDT code.

; The IDT is created in C (idt.c)

; Global symbols so the IDT pointer can be accessed
GLOBAL installIDT
EXTERN IDTPointer

; Make CPU aware of the IDT using the `lidt` instruction
; No need to load any segment selectors this time :)
installIDT:
    lidt [IDTPointer]
    ret