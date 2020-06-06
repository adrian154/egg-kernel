#ifndef __INTERRUPT_H
#define __INTERRUPT_H

// Inlined asm functions, since inline assembly is evil and should be avoided at all costs
static inline void disableInterrupts() {
    __asm__ __volatile__ ("cli");
}

static inline void enableInterrupts() {
    __asm__ __volatile__ ("sti");
}

#endif