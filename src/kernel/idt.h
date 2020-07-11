#ifndef __IDT_H
#define __IDT_H

#include "kernel.h"

// NB: The kernel doesn't use task gates, so their existence is totally ignored.
// This makes the IDT entry structures significantly simpler

struct IDTEntry {
    uint16_t offsetLow;         // lower 16 bits of interrupt handler offset
    uint16_t segmentSelector;   // segment selector
    uint8_t zero;               // unused fields
    uint8_t flags;              // flags - type, other info about gate
    uint16_t offsetHigh;        // upper 16 bits of offset 
}__attribute__((packed));

struct IDTDescriptor {
    uint16_t size;              // number of entries in the IDT
    uint32_t offset;            // offset of the IDT in physical memory
}__attribute__((packed));

// Bitmasks to manipulate IDT fields
#define IDT_ENTRY_PRESENT           0b10000000
#define IDT_ENTRY_RING0             0b00000000
#define IDT_ENTRY_RING1             0b00100000
#define IDT_ENTRY_RING2             0b01000000
#define IDT_ENTRY_RING3             0b01100000
#define IDT_ENTRY_32BIT             0b00001000
#define IDT_ENTRY_16BIT             0b00000000
#define IDT_ENTRY_INTERRUPT_GATE    0b00000110
#define IDT_ENTRY_TRAP_GATE         0b00000111

// Our IDT occupies all 256 entries (idt.c)
extern struct IDTEntry IDT[256];
extern struct IDTDescriptor IDTPointer;

extern void installIDT();
extern void setupIDT();
extern void addIDTEntry(int index, uint32_t offset, uint16_t selector, uint8_t flags);

#endif