/*
 * It's a good idea to do all our physical memory management in one place.
 * This allocator keeps track of physical memory on a per-page basis.
 * Internally, the allocator keeps a stack of free pages, so both allocations and frees are O(1).
 */
#include "physalloc.h"
#include "init.h"
#include "mmap.h"
#include "string.h"
#include "terminal.h"

uint32_t *free_pages_stack_base;
uint32_t *free_pages_stack;
size_t free_pages_stack_size;

void physalloc_init(struct EnvironmentData *envdata) {

    /* place the free pages stack somewhere */
    free_pages_stack_base = NULL; /* FIXME */
    free_pages_stack = free_pages_stack_base;

    /* work through the memory map, pushing free addresses onto the stack */
    /* we assume the memory map returned by the BIOS is sane, i.e. no overlapping regions, no garbage entries, etc. */
    struct MemoryMapEntry *mmap = (struct MemoryMapEntry *)envData->memoryMap;
    for(int i = 0; i < envdata->numMemoryMapEntries; i++) {

        /* only consider memory explicitly marked free as available for use */
        if(mmap->type == MMAP_FREE) {

            /* ignore entries above 4GiB, we're 32-bit only for now */
            if(mmap->base + mmap->length > (uint64_t)0xffffffff) {
                continue;
            }

            /* make sure the start/end addresses are 4KiB aligned; all pages returned by the allocator must be fully free */
            uint32_t start = mmap->base;
            uint32_t end = mmap->base + mmap->length;
            
            if(!(start & 0xfff)) {
                start = (start >> 12 + 1) << 12;
            }

            if(!(end & 0xfff)) {
                end = end & 0xfffff000;
            }

            /* push pages */
            for(uint32_t addr = start; addr <= end; addr += 4096) {
                *(free_pages_stack++) = addr;
            }

        }

        mmap++;

    }

    /* TODO: mark pages occupied by the kernel and the stack as occupied */
    /* TODO: set bitmapPhysicalEnd */
    /* envData->bitmapPhysicalEnd = end; */

}

void *physalloc_alloc() {
    return *(free_pages_stack--);
}

/* because physalloc_free() simply pushes the page back onto the stack, double-frees are *very* bad */
/* if a page is double-freed, it may be returned by physalloc_alloc() multiple times */
void *physalloc_free(void *ptr) {
    if(ptr != NULL) {
        *(free_pages_stack++) = (uint32_t)ptr;
    }
}