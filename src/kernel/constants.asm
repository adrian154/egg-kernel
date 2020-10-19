; Constants used in assembly files
; DO NOT PUT ACTUAL CODE HERE AS IT WILL CAUSE TROUBLE

%ifndef __CONSTANTS
%define __CONSTANTS

; See gdt.h for explanation of selector values
CODE_SEG equ 0x08
DATA_SEG equ 0x10
USER_CODE_SEG equ 0x1B
USER_DATA_SEG equ 0x23
TSS_SEG equ 0x2B

%endif