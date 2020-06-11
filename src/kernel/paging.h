#ifndef __PAGING_H
#define __PAGING_H

#include "kernel.h"

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

extern void loadPageDirectory(uint32_t *pageDirectory);
extern void enablePaging();
extern void setupPaging();

#endif