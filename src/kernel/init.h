#ifndef __INIT_H
#define __INIT_H

// Struct passed by bootloader (plus a few extra fields)
struct EnvironmentData {
    uint8_t diskNumber;
    uint16_t numMemoryMapEntries;
    void *memoryMap;
    uint32_t kernelPhysicalStart;
    uint32_t kernelPhysicalEnd;
}__attribute__((packed));

#endif