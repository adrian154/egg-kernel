#include "physalloc.h"
#include "init.h"
#include "mmap.h"
#include "terminal.h"

uint32_t *pageBitmap;

void setupPhysicalAlloc(struct EnvironmentData *envData) {

    // First, determine how large the memory is in pages by parsing the memory map
    uint64_t maxFreeAddr = 0;

    struct MemoryMapEntry *mmap = (struct MemoryMapEntry *)envData->memoryMap;
    for(int i = 0; i < envData->numMemoryMapEntries; i++) {
        if(mmap->type == MMAP_FREE) {
            uint64_t limit = mmap->base + mmap->length;
            if(limit > maxFreeAddr) {
                maxFreeAddr = limit;
            } 
        }
        mmap++;
    }

    // 1 uint32_t = 4K * 32 bits
    int bitmapSize = mmap / (4096 * 32);

    

}