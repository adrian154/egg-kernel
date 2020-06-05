#include "kernel.h"
#include "terminal.h"
#include "gdt.h"

void cmain(uint32_t kernelPhysicalStart, uint32_t kernelPhysicalEnd) {
    
    initTerminal();
    
    print("egg kernel started\n");

    setupGDT();
    print("GDT setup was successful\n");

    // infinite loop so CPU doesn't go crazy
    for(;;);

}
