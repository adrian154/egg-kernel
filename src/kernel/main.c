#include "exception.h"
#include "interrupt.h"
#include "physalloc.h"
#include "terminal.h"
#include "kernel.h"
#include "string.h"
#include "paging.h"
#include "print.h"
#include "init.h"
#include "mmap.h"
#include "gdt.h"
#include "idt.h"
#include "irq.h"

void setupEarly(struct EnvironmentData *envData) {

    clearTerminal();
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
        printf("base=0x%xq, length=0x%xq, type=%d, ACPI=0x%xb\n", mmapEnt->base, mmapEnt->length, mmapEnt->type, mmapEnt->ACPIAttributes);
        mmapEnt++;
    }

}

void printEnvData(struct EnvironmentData *envData) {
    printf("disk number = 0x%xb\n", envData->diskNumber);
    printf("kernel physical start  = 0x%xd\n", envData->kernelPhysicalStart);
    printf("kernel physical end    = 0x%xd\n", envData->kernelPhysicalEnd);
    printf("bitmap physical end    = 0x%xd\n", envData->bitmapPhysicalEnd);
    printf("kernel interrupt stack = 0x%xd\n", envData->interruptStack);
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