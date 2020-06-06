#ifndef __EXCEPTION_H
#define __EXCEPTION_H

#include "kernel.h"

// Data on the stack before C exception handler starts working
struct ExceptionFrame {
    uint32_t DS, ES, FS, GS;                                // Also pushed
    uint32_t EDI, ESI, EBP, kernelESP, EBX, EDX, ECX, EAX;  // Pushed by PUSHA in common exception handler 
    uint32_t interruptNumber, errorCode;                    // Pushed by exception handlers
    uint32_t EFLAGS, CS, EIP;                               // Pushed by processor automagically
    uint32_t userSS, userESP;                               // Might be pushed if the interrupted code was usermode; only access if you know this for certain! 
};

// Interrupt handlers declared in exception.asm
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

extern void exceptionHandler(struct ExceptionFrame *frame);
extern void setupExceptionHandlers();

#endif