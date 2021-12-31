#ifndef __GDT_H
#define __GDT_H

#include "kernel.h"
#include "tss.h"
#include "interrupt.h"
#include "init.h"

// the structure of each individual GDT entry is bafflingly complicated
// for example the upper 4 bits of the limit are packed with a bunch of other bitfields
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

// --- GDT access bitfields

// `type` field
// The value of this field is interpreted differently for code/data descriptors and system descriptors
// code/data descriptor type fields:
#define GDT_ACCESS_CODE_SEGMENT        0x1A // readable code segment
#define GDT_ACCESS_DATA_SEGMENT        0x12 // writeable data segment
#define GDT_ACCESS_TSS                 0x09

// system descriptor type fields:
// TODO

// privilege level
#define GDT_ACCESS_RING0               0x00
#define GDT_ACCESS_RING3               0x60

// present flag
#define GDT_ACCESS_PRESENT             0x80

// --- GDT flags bitfields

// default operation size flag
#define GDT_FLAGS_16BIT                0x00
#define GDT_FLAGS_32BIT                0x40
#define GDT_FLAGS_BYTE_GRANULARITY     0x00
#define GDT_FLAGS_4K_GRANULARITY       0x80

// --- TSS descriptor flags
#define GDT_ACCESS_TSS                 0x09
#define GDT_ACCESS_TSS_BUSY            0x02

// simple static GDT
#define NUM_GDT_ENTRIES 6
#define GDT_CODE_SELECTOR              0x08    // PL=0, Table=GDT, Index=1
#define GDT_DATA_SELECTOR              0x10    // PL=0, Table=GDT, Index=2
#define GDT_USER_CODE_SELECTOR         0x1B    // PL=3, Table=GDT, Index=3
#define GDT_USER_DATA_SELECTOR         0x23    // PL=3, Table=GDT, Index=4
#define GDT_TSS_SELECTOR               0x2B    // PL=3, Table=GDT, Index=5

extern struct GDTEntry GDT[NUM_GDT_ENTRIES];
extern struct GDTDescriptor GDTPointer;
extern struct TSSEntry kernelTSSEntry;

extern void installGDT();
extern void setupGDT(struct EnvironmentData *envData);
extern void setKernelStack(uint32_t stack);

#endif