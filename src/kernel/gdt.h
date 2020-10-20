#ifndef __GDT_H
#define __GDT_H

#include "kernel.h"
#include "tss.h"
#include "interrupt.h"
#include "init.h"

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

// Bitmasks to manipulate GDT fields
#define GDT_CODE_SEG_READABLE       0b00000010
#define GDT_DATA_SEG_WRITEABLE      0b00000010
#define GDT_ACCESS_NO_READWRITE     0b00000000
#define GDT_ACCESS_GROWS_UP         0b00000000
#define GDT_ACCESS_GROWS_DOWN       0b00000100
#define GDT_ACCESS_EXECUTABLE       0b00001000
#define GDT_ACCESS_CODE_OR_DATA     0b00010000
#define GDT_OTHER                   0b00000000
#define GDT_ACCESS_PRESENT          0b10000000

// Flags for TSS descriptors
#define GDT_ACCESS_TSS              0b00001001
#define GDT_ACCESS_TSS_BUSY         0b00000010

#define GDT_ACCESS_RING0            0b00000000
#define GDT_ACCESS_RING1            0b00100000
#define GDT_ACCESS_RING2            0b01000000
#define GDT_ACCESS_RING3            0b01100000

#define GDT_FLAGS_16BIT             0b00000000
#define GDT_FLAGS_32BIT             0b01000000
#define GDT_FLAGS_BYTE_GRANULARITY  0b00000000
#define GDT_FLAGS_4K_GRANULARITY    0b10000000

// Selectors
#define GDT_CODE_SELECTOR           0x08    // PL=0, Table=GDT, Index=1
#define GDT_DATA_SELECTOR           0x10    // PL=0, Table=GDT, Index=2
#define GDT_USER_CODE_SELECTOR      0x1B    // PL=3, Table=GDT, Index=3
#define GDT_USER_DATA_SELECTOR      0x23    // PL=3, Table=GDT, Index=4
#define GDT_TSS_SELECTOR            0x2B    // PL=3, Table=GDT, Index=5

// There are 3 GDT entries (gdt.c)
#define NUM_GDT_ENTRIES 6

extern struct GDTEntry GDT[NUM_GDT_ENTRIES];
extern struct GDTDescriptor GDTPointer;
extern struct TSSEntry kernelTSSEntry;

extern void installGDT();
extern void setupGDT(struct EnvironmentData *envData);
extern void setKernelStack(uint32_t stack);

#endif