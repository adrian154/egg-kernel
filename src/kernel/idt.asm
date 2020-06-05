; IDT code.

; Global symbols so the IDT pointer can be accessed
GLOBAL installIDT
EXTERN IDTPointer

; Makes CPU aware of the IDT
; No need to load any selectors this time :)
installIDT:
    lidt [IDTPointer]
    ret