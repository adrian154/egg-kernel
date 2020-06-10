#ifndef __INIT_H
#define __INIT_H

struct EnvironmentData {
    uint8_t diskNumber;
    uint16_t numMemoryMapEntries;
    void *memoryMap;
}__attribute__((packed));

#endif