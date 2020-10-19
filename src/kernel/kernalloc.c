#include "kernalloc.h"

// The physical allocator keeps track of physical pages. 
// However, those physical pages need to be allocated to virtual addresses to be used.
// This allocator manages the segment of virtual address space that is reserved for the kernel.
// This allows the kernel to dynamically allocate memory at runtime.
uint8_t *lastSearched;

bool isMapped(void *addr) {
    uint32_t PDEIdx = getPDEIndex(addr);
    if(PDEIdx & PDE_PRESENT) {
        return getPTEIndex(addr) & PTE_PRESENT;
    } else {    
        return true;
    }

}

void *findFreeRange(unsigned int howMany) {

    void *freeStart = NULL;
    unsigned int accum = 0;

    while(true) {

        if(!isMapped(lastSearched)) {
            
            if(freeStart == NULL) {
                freeStart = lastSearched;
            }

            accum++;
            if(accum == howMany) {
                return lastSearched;
            }

        } else {
            freeStart = NULL;
            accum = 0;
        }

        lastSearched += 4096;
        if((uint32_t)lastSearched > KERNEL_RESERVED_MAX) {
            lastSearched = (uint8_t *)0;
        }

    }

}

void *allocVirt(unsigned int pages) {

}

void *freeVirt(void *addr) {

}

void initKernAlloc() {
    lastSearched = (uint8_t *)0;
}