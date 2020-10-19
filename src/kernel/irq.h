#ifndef __IRQ_H
#define __IRQ_H

#include "kernel.h"
#include "interrupt.h"

// Data on the stack before C IRQ handler starts working
// Resembles ExceptionFrame (see exception.h) but lacks `errorCode` since IRQ handlers don't push error codes
struct IRQFrame {

    // Pushed by commonIRQHandler
    // See exception.h/exception.asm for similar code with better explanations
    uint32_t GS;
    uint32_t FS;
    uint32_t ES;
    uint32_t DS;
    uint32_t EDI;
    uint32_t ESI;
    uint32_t EBP;
    uint32_t ESP;
    uint32_t EBX;
    uint32_t EDX;
    uint32_t ECX;
    uint32_t EAX;

    // Pushed by each exception handler
    uint32_t interruptNumber;

    struct InterruptFrame interruptFrame;

}__attribute__((packed));

// Typedef function pointer since function pointers in C are abhorrent
// This code is terrible
typedef void (*IRQHandler)(struct IRQFrame *);

extern void setIRQHandler(int irq, IRQHandler handler);
extern void clearIRQHandler(int irq);
extern void setupIRQs();

#endif