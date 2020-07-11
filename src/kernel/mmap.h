#ifndef __MMAP_H
#define __MMAP_H

#define MMAP_FREE 1
#define MMAP_RESERVED 2
#define MMAP_ACPI_RECLAIMABLE 3
#define MMAP_ACPI_NVS 4

// E820 memory map entry
struct MemoryMapEntry {
    uint64_t base;
    uint64_t length;
    uint32_t type;
    uint32_t ACPIAttributes;
}__attribute__((packed));

#endif