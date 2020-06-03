#include "kernel.h"
#include "terminal.h"

void cmain(uint32_t kernelPhysicalStart, uint32_t kernelPhysicalEnd) {
    initTerminal();
    print("hello worl!\nnewline\nnew line (2)");
    for(;;);
}
