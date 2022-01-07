#ifndef __INTERRUPT_H
#define __INTERRUPT_H

// saved by exception/irq-handling code
struct InterruptContext {

    // segments
    uint32_t GS;
    uint32_t FS;
    uint32_t ES;
    uint32_t DS;

    // registers (pusha)
    uint32_t EDI;
    uint32_t ESI;
    uint32_t EBP;
    uint32_t ESP;
    uint32_t EBX;
    uint32_t EDX;
    uint32_t ECX;
    uint32_t EAX;

};

// fields pushed by the processor
struct CPUInterruptFrame {
    
    uint32_t EIP;
    uint32_t CS;
    uint32_t EFLAGS;

    // these fields only exist if the interrupt triggered a privilege level change
    uint32_t userESP;
    uint32_t userSS;

};

// Functions to disable/enable interrupts from C
static inline void disableInterrupts() {
    __asm__ __volatile__ ("cli");
}

static inline void enableInterrupts() {
    __asm__ __volatile__ ("sti");
}

#endif
