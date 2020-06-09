#ifndef __IRQ_H
#define __IRQ_H

#include "kernel.h"

// Data on the stack before C IRQ handler starts working
// Resembles ExceptionFrame (see exception.h) but lacks `errorCode` since IRQ handlers don't push error codes
struct IRQFrame {
    uint32_t DS, ES, FS, GS;                                        // Also pushed
    uint32_t EDI, ESI, kernelEBP, kernelESP, EBX, EDX, ECX, EAX;    // Pushed by PUSHA in common exception handler 
    uint32_t interruptNumber;                                       // Pushed by exception handlers
    uint32_t EFLAGS, CS, EIP;                                       // Pushed by processor automagically
    uint32_t userSS, userESP;                                       // Might be pushed if the interrupted code was usermode; only access if you know this for certain! 
}__attribute__((packed));

// Typedef function pointer since function pointers in C are abhorrent
// This code is terrible
typedef void (*IRQHandler)(struct IRQFrame *);

extern void setIRQHandler(int irq, IRQHandler handler);
extern void clearIRQHandler(int irq);
extern void setupIRQs();

#endif