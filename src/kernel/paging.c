#include "paging.h"
#include "physalloc.h"

// Static page directory
// Other paging structures are dynamically allocated, but there will only ever be one PDE
uint32_t pageDirectory[1024] __attribute__((aligned(4096)));

void setupPaging() {

    // Clear paging directory
    for(int  i = 0; i < 1024; i++) {
        pageDirectory[i] = PDE_NOT_PRESENT | PDE_SUPERVISOR | PDE_READ_WRITE;
    }

    // Identity page first 4M for simplicity
    uint32_t *testTable = allocPage();
    for(int i = 0; i < 1024; i++) {
        testTable[i] = (i << 12) | PTE_SUPERVISOR | PTE_READ_WRITE | PTE_PRESENT;
    }
    pageDirectory[0] = (uint32_t)testTable | PDE_PRESENT | PDE_SUPERVISOR | PDE_READ_WRITE;

    loadPageDirectory(pageDirectory);
    enablePaging();

}

void mapPage(void *physical, void *logical, uint32_t pageAttributes) {

    // todo

}