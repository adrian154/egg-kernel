#ifndef __INTERRUPT_H
#define __INTERRUPT_H

static inline void disableInterrupts() {
    __asm__ __volatile__ ("cli");
}

static inline void enableInterrupts() {
    __asm__ __volatile__ ("sti");
}

#endif
