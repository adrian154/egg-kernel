#ifndef __INIT_H
#define __INIT_H

// Struct passed by bootloader (plus a few extra fields populated at various stages)
struct EnvironmentData {
    uint8_t diskNumber;
    uint16_t numMemoryMapEntries;
    void *memoryMap;
    uint32_t kernelPhysicalStart;
    uint32_t kernelPhysicalEnd;
    uint32_t bitmapPhysicalEnd;
    uint32_t interruptStack;
}__attribute__((packed));

#endif