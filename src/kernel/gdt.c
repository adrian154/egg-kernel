#include "gdt.h"
#include "string.h"

/*
 * For simplicity, egg-kernel only uses paging for memory management.
 * However, segmentation is always enabled and it comes *before* paging, so we still need to set it up.
 * This code creates a few 'bare minimum' segments that let us access a flat 4G memory space.
 */

struct GDTEntry GDT[NUM_GDT_ENTRIES];
struct TSSEntry kernelTSSEntry;
struct GDTDescriptor GDTPointer;

// add entry to GDT
void addGDTEntry(int index, uint32_t base, uint32_t limit, uint8_t access, uint8_t flags) {

    // set up segment base address
    GDT[index].baseLow = base & 0xFFFF;             // Lower 16 bits
    GDT[index].baseMiddle = (base >> 16) & 0xFF;    // Middle 8 bits
    GDT[index].baseHigh = (base >> 24) & 0xFF;      // Upper 8 bits

    // set up segment limit
    // because the GDT only has 20 bits for the limit address, we need 2^12 = 4K bytes of granularity to fill the full 4GB address space
    limit >>= 12;
    GDT[index].limitLow = limit & 0xFFFF;               // Lower 16 bits
    GDT[index].flagsAndLimit = (limit >> 16) & 0x0F;    // Upper 4 bits

    // fill in the other bitfields
    GDT[index].access = access;
    GDT[index].flagsAndLimit |= flags & 0xF0;

}

// Set up the GDT
void setupGDT(struct EnvironmentData *envData) {

    // the size field in the GDT descriptor is subtracted by 1
    // this is because a uint16_t can only store 0..65535 but the max size of a GDT is 65536
    GDTPointer.offset = (uint32_t)&GDT;
    GDTPointer.size = sizeof(struct GDTEntry) * NUM_GDT_ENTRIES - 1;

    // null descriptor (required by processor)
    addGDTEntry(0, 0, 0, 0, 0);

    // kenel code segment
    addGDTEntry(
        1,
        0x00000000,
        0xFFFFFFFF,
        GDT_ACCESS_PRESENT | GDT_ACCESS_RING0 | GDT_ACCESS_CODE_SEGMENT,
        GDT_FLAGS_4K_GRANULARITY | GDT_FLAGS_32BIT
    );

    // kernel data segment
    addGDTEntry(
        2,
        0x00000000,
        0xFFFFFFFF,
        GDT_ACCESS_PRESENT | GDT_ACCESS_RING0 | GDT_ACCESS_DATA_SEGMENT,
        GDT_FLAGS_4K_GRANULARITY | GDT_FLAGS_32BIT
    );

    // User code segment
    addGDTEntry(
        3,
        0x00000000,
        0xFFFFFFFF,
        GDT_ACCESS_PRESENT | GDT_ACCESS_RING3 | GDT_ACCESS_CODE_SEGMENT,
        GDT_FLAGS_4K_GRANULARITY | GDT_FLAGS_32BIT
    );

    // User data segment
    addGDTEntry(
        4,
        0x00000000,
        0xFFFFFFFF,
        GDT_ACCESS_PRESENT | GDT_ACCESS_RING3 | GDT_ACCESS_DATA_SEGMENT,
        GDT_FLAGS_4K_GRANULARITY | GDT_FLAGS_32BIT
    );

    // TSS segment
    // The EXECUTABLE flag actually determines whether the TSS is 32 or 16 bit
    // Information on TSS descriptors in the GDT is scarce outside of the Intel manuals and it's late ;)
    addGDTEntry(
        5,
        (uint32_t)&kernelTSSEntry,
        sizeof(kernelTSSEntry) - 1,
        GDT_ACCESS_PRESENT | GDT_ACCESS_RING0 | GDT_ACCESS_TSS,
        GDT_FLAGS_BYTE_GRANULARITY // our TSS entry isn't 4K aligned, so use byte granularity
    );

    // set up kernel TSS 
    memset(&kernelTSSEntry, 0, sizeof(struct TSSEntry));

    // when an interrupt occurs, the CPU restores the kernel stack based on the values stored in the TSS
    kernelTSSEntry.SS0 = GDT_DATA_SELECTOR;
    kernelTSSEntry.ESP0 = envData->interruptStack;

    // tell the CPU to use the new GDT + TSS
    installGDT();
    installTSS();

}
