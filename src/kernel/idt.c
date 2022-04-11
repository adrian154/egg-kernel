#include "idt.h"
#include "string.h"

// This code controls the Interrupt Descriptor Table (IDT), which lets the kernel define interrupt handlers.
// Define a 256-entry IDT for 256 possible interrupts
struct IDTEntry IDT[256];
struct IDTDescriptor IDTPointer;

// helper: add entry to IDT
void addIDTEntry(int index, uint32_t offset, uint16_t selector, uint8_t flags) {
    IDT[index].flags = flags;
    IDT[index].zero = 0;
    IDT[index].offsetHigh = offset >> 16;
    IDT[index].offsetLow = offset & 0xFFFF;
    IDT[index].segmentSelector = selector;
} 

void setupIDT() {

    // set up IDT pointer
    IDTPointer.offset = (uint32_t)&IDT;
    IDTPointer.size = sizeof(struct IDTEntry) * 256 - 1;

    // zero out IDT
    memset(&IDT, 0, sizeof(struct IDTEntry) * 256);

    // inform the CPU about the new IDT
    installIDT();

}