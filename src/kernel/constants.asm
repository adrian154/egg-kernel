; Constants used in assembly files
; DO NOT PUT ACTUAL CODE HERE AS IT WILL CAUSE TROUBLE

%ifndef __CONSTANTS
%define __CONSTANTS

CODE_SEG equ 0x08   ; PL=00 (ring0), Table=0 (GDT), Index=1 (first entry in GDT)
DATA_SEG equ 0x10   ; PL=00 (ring0), Table=0 (GDT), Index=2 (second entry in GDT)

%endif