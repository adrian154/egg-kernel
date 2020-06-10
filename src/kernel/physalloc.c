#include "physalloc.h"
#include "init.h"
#include "mmap.h"
#include "string.h"

// page-frame bitmap
uint32_t *pageBitmap;

// size of the page bitmap in uint32_t's (size in bytes = this x 4)
size_t pageBitmapSizeU32;

void setupPhysicalAlloc(struct EnvironmentData *envData) {

    // Utility.
    uint32_t MAX_U32 = ~0;

    // First, determine how large the memory is in pages by parsing the memory map
    uint64_t maxFreeAddr = 0;

    struct MemoryMapEntry *mmap = (struct MemoryMapEntry *)envData->memoryMap;
    for(int i = 0; i < envData->numMemoryMapEntries; i++) {
        if(mmap->type == MMAP_FREE) {
            uint64_t limit = mmap->base + mmap->length;
            if(limit > maxFreeAddr && mmap->base < MAX_U32 && limit < MAX_U32) {
                maxFreeAddr = limit;
            }
        }
        mmap++;
    }

    // 1 uint32_t = 4K * 32 bits
    pageBitmapSizeU32 = maxFreeAddr / (4096 * 32);

    // place bitmap right above kernel, aligned to 4 bytes
    pageBitmap = (uint32_t *)(envData->kernelPhysicalEnd % 4 == 0 ? envData->kernelPhysicalEnd : envData->kernelPhysicalEnd + 4 - (envData->kernelPhysicalEnd % 4));

    // Set bitmap to all 1's for now
    // This strategy helps avoid issues with shitty maps reported by e820
    memset(pageBitmap, 0xFF, pageBitmapSize * 4);

    // iterate through mmap, "free" up regions that are marked as free
    for(int i = 0; i < envData->numMemoryMapEntries; i++) {
        if(mmap->type == MMAP_FREE) {
            
        }
    }

    // mark pages occupied by kernel and the page bitmap as occupied

}