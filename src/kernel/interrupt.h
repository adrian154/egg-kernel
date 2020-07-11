#ifndef __INTERRUPT_H
#define __INTERRUPT_H

// Functions to disable/enable interrupts from C
static inline void disableInterrupts() {
    __asm__ __volatile__ ("cli");
}

static inline void enableInterrupts() {
    __asm__ __volatile__ ("sti");
}

#endif
