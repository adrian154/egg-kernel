#ifndef __PHYSALLOC_H
#define __PHYSALLOC_H

#include "kernel.h"
#include "init.h"

extern void setupPhysicalAlloc(struct EnvironmentData *envData);
extern void *allocPhysPage();
extern void freePage(void *page);

#endif