#include "physalloc.h"
#include "init.h"
#include "mmap.h"
#include "string.h"
#include "terminal.h"

// page-frame bitmap
uint32_t *pageBitmap;

// size of the page bitmap in uint32_t's (size in bytes = this x 4)
size_t pageBitmapSizeU32;

// Internal functions to manipulate page bitmap
static inline void iFreePage(uint32_t pageNumber) {
    pageBitmap[pageNumber / 32] &= ~((uint32_t)1 << (pageNumber % 32));
}

static inline void iAllocPage(uint32_t pageNumber) {
    pageBitmap[pageNumber / 32] |= (uint32_t)1 << (pageNumber % 32);
}

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
            } else if(mmap->base < MAX_U32 && limit > MAX_U32) {
                maxFreeAddr = MAX_U32;
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
    memset(pageBitmap, 0xFF, pageBitmapSizeU32 * 4);

    // iterate through mmap, "free" up regions that are marked as free
    mmap = (struct MemoryMapEntry *)envData->memoryMap;
    for(int i = 0; i < envData->numMemoryMapEntries; i++) {
        if(mmap->type == MMAP_FREE) {
            
            uint64_t base64 = mmap->base;
            uint64_t limit64 = mmap->base + mmap->length;
            
            // If the entire region is above the 32-bit limit, ignore it
            if(base64 > MAX_U32) {
                continue;
            }

            // Calculate the effective start/end of the region
            uint32_t base32 = (uint32_t)base64;
            uint32_t limit32 = (uint32_t)limit64;
            if(base64 < MAX_U32 && limit64 > MAX_U32) {
                limit32 = MAX_U32;
            }

            // Ignore partially free pages at the end of each region
            uint32_t startPage = ~(base32 & 0xFFF) ? (base32 >> 12) : (base32 >> 12) + 1;
            uint32_t endPage = limit32 >> 12;
            
            print("startpage="); printHexInt(startPage); print(", endpage="); printHexInt(endPage); putChar('\n');

            for(uint32_t j = startPage; j <= endPage; j++) {
                iFreePage(j);
            }

        }

        mmap++;
    }

    // mark pages occupied by kernel and the page bitmap as occupied
    uint32_t start = (envData->kernelPhysicalStart >> 12) + 1;

}

void *allocPage() {

    for(size_t i = 0; i < pageBitmapSizeU32; i++) {
        
        // Check if there are any free pages in the bitmap
        if(~pageBitmap[i] != 0) {

            // Loop through bits, find free page
            for(int j = 0; j < 32; j++) {
                if(!(pageBitmap[i] & ((uint32_t)1 << j))) {
                    iAllocPage(i * 32 + j);
                    return (void *)((i * 32 + j) * 4096);
                }
            }

        }

    }

    // no pages left (out of memory!)
    // this is very bad, replace with something else eventually
    return NULL;

}

void freePage(void *page) {
    iFreePage((uint32_t)page / 4096);
}