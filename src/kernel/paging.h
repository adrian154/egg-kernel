#ifndef __PAGING_H
#define __PAGING_H

#include "kernel.h"

// Bitmasks to manipulate page directories and tables
#define PDE_PRESENT         0b00000001
#define PDE_NOT_PRESENT     0b00000000
#define PDE_READ_WRITE      0b00000010
#define PDE_READ_ONLY       0b00000000
#define PDE_USER            0b00000100
#define PDE_SUPERVISOR      0b00000000
#define PDE_WRITETHROUGH    0b00001000
#define PDE_WRITEBACK       0b00000000
#define PDE_CACHE_DISABLED  0b00010000
#define PDE_CACHE_ENABLED   0b00000000
#define PDE_SIZE_4M         0b10000000
#define PDE_SIZE_4K         0b00000000

#define PTE_PRESENT         0b00000001
#define PTE_NOT_PRESENT     0b00000000
#define PTE_READ_WRITE      0b00000010
#define PTE_READ_ONLY       0b00000000
#define PTE_USER            0b00000100
#define PTE_SUPERVISOR      0b00000000
#define PTE_WRITETHROUGH    0b00001000
#define PTE_WRITEBACK       0b00000000
#define PTE_CACHE_DISABLED  0b00010000
#define PTE_CACHE_ENABLED   0b00000000

// Bitmasks to help with extracting page bits
#define ADDR_HI10_MASK      0xFFC00000
#define ADDR_MID10_MASK     0xFFC00
#define ADDR_LOW12_MASK     0xFFF
#define ADDR_HI20_MASK      0xFFFFF000

extern uint32_t pageDirectory[1024];

// Utility functions to help isolate PDE/PTE entry indexes for a logical address
static inline uint32_t getPDEIndex(void *logical) {
    uint32_t addr = (uint32_t)logical; 
    return (addr & ADDR_HI10_MASK) >> 22;
}

static inline uint32_t getPTEIndex(void *logical) {
    uint32_t addr = (uint32_t)logical;
    return (addr & ADDR_MID10_MASK) >> 12;
}

static inline uint32_t getOffset(void *logical) {
    uint32_t addr = (uint32_t)logical;
    return addr & ADDR_LOW12_MASK;
}

static inline uint32_t *getPageTable(void *logical) {
    int idx = getPDEIndex(logical);
    if(pageDirectory[idx] & PDE_PRESENT) {
        return (uint32_t *)(pageDirectory[idx] & ADDR_HI20_MASK);
    } else {
        return NULL;
    }
}

static inline void *getPhysMapping(void *logical) {
    uint32_t *pageTable = getPageTable(logical);
    if(pageTable == NULL) return NULL;
    int idx = getPTEIndex(logical);
    if(pageTable[idx] & PTE_PRESENT) {
        return (void *)(pageTable[idx] & ADDR_HI20_MASK);
    } else {
        return NULL;
    }
}

extern void loadPageDirectory(uint32_t *pageDirectory);
extern void enablePaging();
extern void setupPaging();
extern void invalidateMapping(void *pageMapping);
extern void flushAllPages();

#endif