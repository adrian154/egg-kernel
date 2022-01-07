#ifndef __IRQ_H
#define __IRQ_H

#include "kernel.h"
#include "interrupt.h"

// IRQFrame represents the data present on the stack when `mainIRQHandler` is called
struct IRQFrame {
    struct InterruptContext ctx;
    uint32_t IRQNumber;
    struct CPUInterruptFrame cpuFrame;
}__attribute__((packed));

// Typedef function pointer since function pointers in C are abhorrent
// This code is terrible
typedef void (*IRQHandler)(struct IRQFrame *);

extern void setIRQHandler(int irq, IRQHandler handler);
extern void clearIRQHandler(int irq);
extern void setupIRQs();

#endif