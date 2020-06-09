#include "kernel.h"
#include "terminal.h"
#include "gdt.h"
#include "idt.h"
#include "exception.h"
#include "interrupt.h"
#include "irq.h"

void testHandler(struct IRQFrame *frame) {
    print("Hello Worl");
}

void cmain(uint32_t kernelPhysicalStart, uint32_t kernelPhysicalEnd) {
    
    disableInterrupts();

    initTerminal();
    
    print("egg kernel started\n");

    setupGDT();
    print("GDT setup was successful\n");

    setupIDT();
    print("IDT setup was successful\n");

    setupExceptionHandlers();
    print("Exception handler setup was successful\n");

    setupIRQs();
    print("IRQ handler setup was successful\n");

    // Ready to enable interrupts at this point
    enableInterrupts();

    // infinite loop so CPU doesn't start executing junk
    for(;;) {
        __asm__ ("hlt");
    }

}
