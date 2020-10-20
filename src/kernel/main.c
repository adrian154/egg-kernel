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
#include "kernalloc.h"

void cmain(struct EnvironmentData *envDataOld, uint32_t kernelPhysicalStart, uint32_t kernelPhysicalEnd) {
    
    // Copy passed envData into a new envData on the stack since the old one is lying around in free memory that may be overwritten
    struct EnvironmentData envData;
    memcpy(envDataOld, &envData, sizeof(struct EnvironmentData));    

    // Do the same for the memory map since it too is in potentially free memory
    // Unfortunately a VLA must be used here, but it's probably fine...
    struct MemoryMapEntry newmmap[envData.numMemoryMapEntries];
    memcpy(envData.memoryMap, newmmap, sizeof(struct MemoryMapEntry) * envData.numMemoryMapEntries);

    // Update envdata entry, since the memory map has moved
    envData.memoryMap = newmmap;

    // Fill in some fields in envData
    // Some other fields are also filled in by the physical allocator initialization code
    envData.kernelPhysicalStart = kernelPhysicalStart;
    envData.kernelPhysicalEnd = kernelPhysicalEnd;

    disableInterrupts();

    // set up terminal and print a little hello message
    initTerminal();
    print("egg kernel started\n");

    // do some early environment setup
    setupGDT();
    print("GDT setup was successful\n");
    print("interrupt stack is at 0x"); printHexInt((uint32_t)interrupt_stack);

    setupIDT();
    print("\nIDT setup was successful\n");

    setupExceptionHandlers();
    print("Exception handler setup was successful\n");

    setupIRQs();
    print("IRQ handler setup was successful\n");

    // Ready to enable interrupts at this point
    enableInterrupts();

    // Print memory map for debugging (generally useful)
    struct MemoryMapEntry *mmapEnt = envData.memoryMap;
    for(int i = 0; i < envData.numMemoryMapEntries; i++) {
        print("base=0x"); printHexLong(mmapEnt->base); print(", length="); printHexLong(mmapEnt->length); print(", type="); printHexByte(mmapEnt->type); print(", ACPI=0x"); printHexByte(mmapEnt->ACPIAttributes); putChar('\n');
        mmapEnt += 1;
    }

    // Set up paging
    setupPhysicalAlloc(&envData);
    setupPaging();

    testEnterUsermode();

    // infinite loop so CPU doesn't start executing junk
    for(;;) {
        __asm__ ("hlt");
    }

}