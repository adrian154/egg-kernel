#include "kernel.h"
#include "terminal.h"
#include "gdt.h"
#include "idt.h"
#include "exception.h"
#include "interrupt.h"
#include "irq.h"
#include "init.h"
#include "mmap.h"
#include "physalloc.h"
#include "string.h"
#include "paging.h"

void cmain(struct EnvironmentData *envDataOld, uint32_t kernelPhysicalStart, uint32_t kernelPhysicalEnd) {
    
    // Copy passed envData into a new envData on the stack since the old one is lying around in free memory that may be overwritten
    struct EnvironmentData envData;
    memcpy(envDataOld, &envData, sizeof(struct EnvironmentData));    

    // Fill in some fields in envData
    envData.kernelPhysicalStart = kernelPhysicalStart;
    envData.kernelPhysicalEnd = kernelPhysicalEnd;

    disableInterrupts();

    // set up terminal and print a little hello message
    initTerminal();
    print("egg kernel started\n");

    // do some early environment setup
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

    // Print memory map for debugging (generally useful)
    struct MemoryMapEntry *mmap = (struct MemoryMapEntry *)envData.memoryMap;
    for(int i = 0; i < envData.numMemoryMapEntries; i++) {
        print("base=0x"); printHexLong(mmap->base); print(", length="); printHexLong(mmap->length); print(", type="); printHexByte(mmap->type); print(", ACPI=0x"); printHexByte(mmap->ACPIAttributes); putChar('\n');
        mmap += 1;
    }

    setupPhysicalAlloc(&envData);
    setupPaging();

    int *test = 0x500000;
    *test = 5;

    // infinite loop so CPU doesn't start executing junk
    for(;;) {
        __asm__ ("hlt");
    }

}