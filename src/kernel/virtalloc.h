#ifndef __VIRTALLOC_H
#define __VIRTALLOC_H

#include "kernel.h"
#include "paging.h"
#include "physalloc.h"

// Maximum address reserved for the kernel
// Must be 4K aligned, lower 12 bits are discarded
#define KERNEL_RESERVED_MAX 0x04000000

extern void setupVirtAlloc();
extern void *getFreeVirtAddr();
extern void *allocVirtPage();
extern void freeVirtPage(void *ptr);

#endif