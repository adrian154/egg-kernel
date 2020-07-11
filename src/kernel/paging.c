#include "paging.h"
#include "physalloc.h"

// Static page directory
// Other paging structures are dynamically allocated, but there will only ever be one PDE
uint32_t pageDirectory[1024] __attribute__((aligned(4096)));

// Bottom 64M of virtual address space is reserved for the OS.
// This can be changed.
uint32_t KERNEL_ADDR_SPACE_SIZE = 64 * 1024 * 1024;

// Initialize paging
void setupPaging() {

    // Clear paging directory
    for(int  i = 0; i < 1024; i++) {
        pageDirectory[i] = PDE_NOT_PRESENT | PDE_SUPERVISOR | PDE_READ_WRITE;
    }

    // Identity page first 4M
    // The other 60M are mapped later on demand
    uint32_t *testTable = allocPage();
    for(int i = 0; i < 1024; i++) {
        testTable[i] = (i << 12) | PTE_SUPERVISOR | PTE_READ_WRITE | PTE_PRESENT;
    }
    pageDirectory[0] = (uint32_t)testTable | PDE_PRESENT | PDE_SUPERVISOR | PDE_READ_WRITE;

    loadPageDirectory(pageDirectory);
    enablePaging();

}