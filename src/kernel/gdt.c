#include "gdt.h"

// Set up another GDT in C since it's more convenient
// Segmentation is deprecated anyways
// The safest/most convenient option is to simply set up a 4G flat memory space

// 5 GDT entries:
// 1 null entry (required)
// 2 ring 0 entries (code/data segment)
struct GDTEntry GDT[NUM_GDT_ENTRIES];
struct GDTDescriptor GDTPointer;

// Add entry to GDT
void addGDTEntry(int index, uint32_t base, uint32_t limit, uint8_t access, uint8_t flags) {

    // Set up base address
    GDT[index].baseLow = base & 0xFFFF;                     // Lower 16 bits
    GDT[index].baseMiddle = (base >> 16) & 0xFF;            // Middle 8 bits
    GDT[index].baseHigh = (base >> 24) & 0xFF;              // Upper 8 bits

    // Also populate limit
    // (Limit is only 20 bits in GDT; you need 4K (2^12) granularity to fill the full 32-bit 4G address space)
    GDT[index].limitLow = (limit & 0xFFFF);                 // Lower 16 bits
    GDT[index].flagsAndLimit = ((limit >> 16) & 0x0F);      // Upper 4 bits

    // Finally, access/flags
    GDT[index].access = access;
    GDT[index].flagsAndLimit |= (flags & 0xF0);

}

// Set up the GDT
void setupGDT() {

    // The size field in the GDT descriptor is subtracted by 1
    // This is because a uint16_t can only store 0..65535 but the max size of a GDT is 65536
    GDTPointer.offset = (uint32_t)&GDT;
    GDTPointer.size = sizeof(struct GDTEntry) * 5 - 1;

    // First, the null descriptor
    addGDTEntry(0, 0, 0, 0, 0);

    // Next, the kernel code segment
    addGDTEntry(
        1,
        0x00000000,
        0xFFFFFFFF,
        GDT_ACCESS_PRESENT | GDT_ACCESS_CODE_OR_DATA | GDT_ACCESS_EXECUTABLE | GDT_CODE_SEG_READABLE | GDT_ACCESS_RING0,
        GDT_FLAGS_4K_GRANULARITY | GDT_FLAGS_32BIT
    );

    // Kernel data segment
    addGDTEntry(
        2,
        0x00000000,
        0xFFFFFFFF,
        GDT_ACCESS_PRESENT | GDT_ACCESS_CODE_OR_DATA | GDT_DATA_SEG_WRITEABLE | GDT_ACCESS_RING0,
        GDT_FLAGS_4K_GRANULARITY | GDT_FLAGS_32BIT
    );

    // Tell CPU about our new GDT
    installGDT();

}