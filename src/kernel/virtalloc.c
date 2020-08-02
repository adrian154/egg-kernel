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

    uint32_t searchAddr = (uint32_t)nextFree;

    do {
        if(getPhysMapping((void *)searchAddr) == NULL) {
            nextFree = (void *)searchAddr;
            return nextFree;
        }

        print("[Incrementing search...]");

        searchAddr += (1 << 12);
        if(searchAddr > KERNEL_RESERVED_MAX)
            searchAddr = 0;
    } while(searchAddr != (uint32_t)nextFree);

    print("[No more virtual addresses!]");

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
    uint32_t pageDirEnt = pageDirectory[getPDEIndex(virtPage)];
    uint32_t *pageTable;
    if(!(pageDirEnt & PDE_PRESENT)) {

        // Allocate new page table
        pageTable = allocPhysPage();

        // Clear out page table
        for(int i = 0; i < 1024; i++) {
            pageTable[i] = PDE_NOT_PRESENT;
        }

    } else {
        pageTable = (uint32_t)(pageDirEnt & ADDR_HI20_MASK);
    }

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