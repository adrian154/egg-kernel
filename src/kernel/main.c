#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "include/print.h"

void cmain(uint32_t kernelPhysicalStart, uint32_t kernelPhysicalEnd) {
    termInit();
    print("Test\ntest");
}
