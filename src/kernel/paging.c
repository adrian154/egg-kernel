#include "paging.h"
#include "physalloc.h"

// Static page directory
// Other paging structures are dynamically allocated, but there will only ever be one PDE
uint32_t pageDirectory[1024] __attribute__((aligned(4096)));

uint32_t *baseTable;

// The kernel page tables are reused in every page table
// However, each process has its own dedicated page table
void setupKernelPages(uint32_t *pageDirectory) {

    pageDirectory[0] = (uint32_t)baseTable | PDE_PRESENT | PDE_SUPERVISOR | PDE_READ_WRITE;

}

// Initialize paging
void setupPaging() {

    // Clear paging directory, initialize with non-present user pages
    for(int  i = 0; i < 1024; i++) {
        pageDirectory[i] = PDE_NOT_PRESENT | PDE_USER | PDE_READ_WRITE;
    }

    // Point last entry at self
    // This maps the last 4M of the virtual address space to just page tables
    pageDirectory[1023] = (uint32_t)pageDirectory | PDE_PRESENT | PDE_SUPERVISOR | PDE_READ_WRITE;

    // Identity page first 4M
    baseTable = allocPhysPage();
    for(int i = 0; i < 1024; i++) {
        baseTable[i] = (i << 12) | PTE_SUPERVISOR | PTE_READ_WRITE | PTE_PRESENT;
    }

    setupKernelPages(pageDirectory);
    
    /*
     * NOTE TO SELF:
     * This is not a good example on how to access page tables.
     * This code sets up paging, so before it all addresses are physical.
     * However, after paging is enabled, the memory returned by the PMM is probably not mapped.
     * Therefore, you must access pages by first going through getTableVirtAddr() (paging.h)
     * Sometimes this will seemingly work since a lot of physical allocations are in the identity-mapped 4M
     * But that will not always be true, and code could mysteriously break!
     */

    loadPageDirectory(pageDirectory);
    enablePaging();

}