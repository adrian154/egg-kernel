#include "kernel.h"
#include "terminal.h"
#include "gdt.h"
#include "idt.h"
#include "exception.h"
#include "interrupt.h"

void cmain(uint32_t kernelPhysicalStart, uint32_t kernelPhysicalEnd) {
    
    initTerminal();
    
    print("egg kernel started\n");

    setupGDT();
    print("GDT setup was successful\n");

    setupIDT();
    print("IDT setup was successful\n");

    setupExceptionHandlers();
    print("Exception handler setup was successful\n");

    // Ready to enable interrupts at this point
    volatile int test = 1/0;
    //enableInterrupts();
    
    // infinite loop so CPU doesn't start executing junk
    for(;;);

}
