#ifndef __GDT_H
#define __GDT_H

#include "kernel.h"

struct GDTEntry {
    uint16_t limitLow;      // lower 16 bits of limit
    uint16_t baseLow;       // lower 16 bits of base
    uint8_t baseMiddle;     // middle 8 bits of base
    uint8_t access;         // access flags (bits in here)
    uint8_t flagsAndLimit;  // some misc bitfields
    uint8_t baseHigh;       // upper 8 bits of base
}__attribute__((packed));

struct GDTDescriptor {
    uint16_t size;          // number of entries in the GDT
    uint32_t offset;        // offset of the GDT in physical memory
}__attribute__((packed));

#define GDT_CODE_SEG_READABLE       0b00000010
#define GDT_DATA_SEG_WRITEABLE      0b00000010
#define GDT_ACCESS_NO_READWRITE     0b00000000
#define GDT_ACCESS_GROWS_UP         0b00000000
#define GDT_ACCESS_GROWS_DOWN       0b00000100
#define GDT_ACCESS_EXECUTABLE       0b00001000
#define GDT_ACCESS_CODE_OR_DATA     0b00010000
#define GDT_OTHER                   0b00000000
#define GDT_ACCESS_PRESENT          0b10000000

#define GDT_ACCESS_RING0            0b00000000
#define GDT_ACCESS_RING1            0b00100000
#define GDT_ACCESS_RING2            0b01000000
#define GDT_ACCESS_RING3            0b01100000

#define GDT_FLAGS_16BIT             0b00000000
#define GDT_FLAGS_32BIT             0b01000000
#define GDT_FLAGS_BYTE_GRANULARITY  0b00000000
#define GDT_FLAGS_4K_GRANULARITY    0b10000000

// See gdt.c for more info on globals
#define NUM_GDT_ENTRIES 3

extern struct GDTEntry GDT[NUM_GDT_ENTRIES];
extern struct GDTDescriptor GDTPointer;

extern void installGDT();

#endif