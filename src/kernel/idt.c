#include "idt.h"
#include "string.h"

// Set up an IDT
// This allows the kernel to define its own interrupt handlers

// The IDT does not strictly have to be 256 entries long, but there's no point in having fewer entries for us
struct IDTEntry IDT[256];
struct IDTDescriptor IDTPointer;

// Add entry to IDT
void addIDTEntry(int index, uint32_t offset, uint16_t selector, uint8_t flags) {

    // Code is largely self explanatory - populate IDT entry with relevant fields
    IDT[index].flags = flags;
    IDT[index].zero = 0;
    IDT[index].offsetHigh = offset >> 16;
    IDT[index].offsetLow = offset & 0xFFFF;
    IDT[index].segmentSelector = selector;

} 

// Set up the IDT
void setupIDT() {

    // Set up IDT pointer first
    IDTPointer.offset = (uint32_t)&IDT;
    IDTPointer.size = sizeof(struct IDTEntry) * 256 - 1;

    // Zero IDT, the processor expects this!
    memset(&IDT, 0, sizeof(struct IDTEntry) * 256);

    // Load the IDT
    installIDT();

}