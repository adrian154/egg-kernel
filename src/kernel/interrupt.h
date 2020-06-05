#ifndef __INTERRUPT_H
#define __INTERRUPT_H

// Inlined asm functions, since inline assembly is evil and should be avoided at all costs
static inline void disableInterrupts() {
    asm("cli");
}

static inline void enableInterrupts() {
    asm("sti");
}

#endif