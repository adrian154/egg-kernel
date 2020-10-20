#ifndef __INTERRUPT_H
#define __INTERRUPT_H

// See init.asm
extern void *interrupt_stack;

// Always pushed by processor on ALL interrupts
struct InterruptFrame {
    
    uint32_t EIP;
    uint32_t CS;
    uint32_t EFLAGS;

    // These fields are only defined if the interrupted code was ring3
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
