#ifndef __EXCEPTION_H
#define __EXCEPTION_H

#include "kernel.h"

// Exception numbers
#define DIVISION_BY_ZERO_EXCEPTION              0
#define DEBUG_EXCEPTION                         1
#define NON_MASKABLE_INTERRUPT_EXCEPTION        2
#define BREAKPOINT_EXCEPTION                    3
#define OVERFLOW_EXCEPTION                      4
#define BOUND_RANGE_EXCEEDED_EXCEPTION          5
#define INVALID_OPCODE_EXCEPTION                6
#define DEVICE_NOT_AVAILABLE_EXCEPTION          7
#define DOUBLE_FAULT_EXCEPTION                  8
#define COPROCESSOR_SEGMENT_OVERRUN_EXCEPTION   9
#define INVALID_TSS_EXCEPTION                   10
#define SEGMENT_NOT_PRESENT_EXCEPTION           11
#define STACK_SEGMENT_FAULT_EXCEPTION           12
#define GENERAL_PROTECTION_FAULT_EXCEPTION      13
#define PAGE_FAULT_EXCEPTION                    14
#define FLOATING_POINT_EXCEPTION                16
#define ALIGNMENT_CHECK_EXCEPTION               17
#define MACHINE_CHECK_EXCEPTION                 18
#define SIMD_FLOATING_POINT_EXCEPTION           19
#define VIRTUALIZATION_EXCEPTION                20

// Data on the stack before C exception handler starts working
struct ExceptionFrame {
    uint32_t GS, FS, ES, DS;                                        // Also pushed
    uint32_t EDI, ESI, kernelEBP, kernelESP, EBX, EDX, ECX, EAX;    // Pushed by PUSHA in common exception handler 
    uint32_t interruptNumber, errorCode;                            // Pushed by exception handlers
    uint32_t EFLAGS, CS, EIP;                                       // Pushed by processor automagically
    uint32_t intSS, intESP;                                         // Interrupted code's SS/ESP 
}__attribute__((packed));

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

// Common exception handler
extern void exceptionHandler(struct ExceptionFrame *frame);
extern void setupExceptionHandlers();

#endif