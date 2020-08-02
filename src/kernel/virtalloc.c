#include "virtalloc.h"

// The physical allocator (physalloc.c) manages physical pages, but it doesn't map any pages
// These functions manage the virtual address space reserved for the kernel
// The virtual address space of individual processes is handled elsewhere (TODO :)

// Pointer to next free virtual address
void *nextFree;

void setupVirtAlloc() {

    // Set up page table entries
    for(int i = 0; i < KERNEL_RESERVED_MAX >> 22; i++) {

        // Don't remap existing mapped pages
        // Make reserved addresss space kernel
        if(!(pageDirectory[i] & PTE_PRESENT)) {
            pageDirectory[i] = PDE_NOT_PRESENT | PDE_SUPERVISOR | PDE_READ_WRITE;
        }

    }

    nextFree = NULL;

}

// Get a free virtual address
void *getFreeVirtAddr() {

    // Save pre-search nextFree so we know when to give up
    uint32_t preSearchAddr = (uint32_t)nextFree;

    while((uint32_t)nextFree != preSearchAddr) {

        if(getPhysMapping(nextFree) == NULL) {
            return nextFree;
        }

        // Increment search address
        preSearchAddr += (1 << 12);
        if(preSearchAddr > KERNEL_RESERVED_MAX)
            preSearchAddr = 0;

    }

    // At this point, PANIC since an exhaustive search has yielded no free virtual addresses.
    // This is very bad.
    return NULL;

}

void *allocVirtPage() {

    // Allocate physical page
    void *physPage = allocPhysPage();

    // Get virtual mapping
    void *virtPage = getFreeVirtAddr();
    
    // Map page as kernel (assuming the PDE is set appropriately by setupVirtAlloc())
    uint32_t *pageTable = getPageTable(virtPage);
    pageTable[getPTEIndex(virtPage)] = (uint32_t)physPage | PDE_PRESENT | PDE_SUPERVISOR | PDE_READ_WRITE;

    // Invalidate any TLB entries so our changes take effect
    invalidateMapping(physPage);

    return virtPage;

}

void freeVirtPage(void *ptr) {

    // Unmap physical page
    void *phys = getPhysMapping(ptr);
    if(phys == NULL) {
        return;
    }

    freePage(phys);

    // Unmap virtual page
    uint32_t *pageTable = getPageTable(ptr);
    int idx = getPTEIndex(ptr);
    pageTable[idx] = PTE_NOT_PRESENT;

}