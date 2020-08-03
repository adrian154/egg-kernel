#include "virtalloc.h"

// The physical allocator (physalloc.c) manages physical pages, but it doesn't map any pages
// These functions manage the virtual address space reserved for the kernel
// The virtual address space of individual processes is handled elsewhere (TODO :)
