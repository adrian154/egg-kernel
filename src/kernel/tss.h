#ifndef  __TSS_H
#define __TSS_H

#include "kernel.h"

// The kernel doesn't use hardware context switching so most of the TSS structure is useless
struct TSSEntry {
    
    uint32_t previousTSS;       // normally TSS is a linked list but alas...
    uint32_t ESP0;              // stack pointer loaded on ring3 -> ring0 transition
    uint32_t SS0;               // stack segment loaded on ring3 -> ring0 transition
    
    // the rest of the fields are included for the sake of completeness and alignment
    uint32_t ESP1;
    uint32_t SS1;
    uint32_t ESP2;
    uint32_t SS2;
    uint32_t CR3;
    uint32_t EIP;
    uint32_t EFLAGS;
    uint32_t EAX;
    uint32_t ECX;
    uint32_t EDX;
    uint32_t EBX;
    uint32_t ESP;
    uint32_t EBP;
    uint32_t ESI;
    uint32_t EDI;
    uint32_t ES;
    uint32_t CS;
    uint32_t SS;
    uint32_t DS;
    uint32_t FS;
    uint32_t GS;
    uint32_t LDTR;
    uint32_t IOPB;
    
}__attribute__((packed));

#endif