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

void setupEarly(struct EnvironmentData *envData) {

    initTerminal();
    print("egg kernel started\n");

    setupGDT(envData);
    print("GDT setup was successful\n");

    setupIDT();
    print("IDT setup was successful\n");

    setupExceptionHandlers();
    print("Exception handler setup was successful\n");

    setupIRQs();
    print("IRQ handler setup was successful\n");
 
}

void printMemoryMap(struct EnvironmentData *envData) {

    struct MemoryMapEntry *mmapEnt = envData->memoryMap;
    for(uint32_t i = 0; i < envData->numMemoryMapEntries; i++) {
        print("base=0x"); printHexLong(mmapEnt->base);
        print(", length="); printHexLong(mmapEnt->length);
        print(", type="); printHexByte(mmapEnt->type);
        print(", ACPI=0x");
        printHexByte(mmapEnt->ACPIAttributes);
        putChar('\n');
        mmapEnt += 1;
    }

}

void printEnvData(struct EnvironmentData *envData) {

    print("[envdata] disk number = 0x"); printHexByte(envData->diskNumber);
    print("\n[envdata] kernel physical start  = 0x"); printHexInt(envData->kernelPhysicalStart);
    print("\n[envdata] kernel physical end    = 0x"); printHexInt(envData->kernelPhysicalEnd);
    print("\n[envdata] bitmap physical end    = 0x"); printHexInt(envData->bitmapPhysicalEnd);
    print("\n[envdata] kernel interrupt stack = 0x"); printHexInt(envData->interruptStack);

}

void cmain(struct EnvironmentData *envDataOld, uint32_t kernelPhysicalStart, uint32_t kernelPhysicalEnd, uint32_t interruptStack) {
    
    // Copy passed envData into a new envData on the stack since the old one is lying around in free memory that may be overwritten
    struct EnvironmentData envData;
    memcpy(&envData, envDataOld, sizeof(struct EnvironmentData));    

    // Do the same for the memory map since it too is in potentially free memory
    // Unfortunately a VLA must be used here, but it's probably fine...
    struct MemoryMapEntry newmmap[envData.numMemoryMapEntries];
    memcpy(newmmap, envData.memoryMap, sizeof(struct MemoryMapEntry) * envData.numMemoryMapEntries);

    // Update envdata entry, since the memory map has moved
    envData.memoryMap = newmmap;

    // Fill in some fields in envData
    // Some other fields are also filled in by the physical allocator initialization code
    envData.kernelPhysicalStart = kernelPhysicalStart;
    envData.kernelPhysicalEnd = kernelPhysicalEnd;
    envData.interruptStack = interruptStack;

    // beware - setupEarly does a ton!
    disableInterrupts();
    setupEarly(&envData);
    enableInterrupts();

    // Print debug info
    printMemoryMap(&envData);

    // Set up paging
    setupPhysicalAlloc(&envData);
    setupPaging();

    // Print debug info
    printEnvData(&envData);

    // infinite loop so CPU doesn't start executing junk
    for(;;) {
        __asm__ ("hlt");
    }

}