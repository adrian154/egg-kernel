#ifndef __STRING_H
#define __STRING_H

#include "kernel.h"

static inline unsigned int strlen(const char *str) {
    unsigned int length = 0;
    for(const char *ptr = str; ptr[length] != '\0'; ptr++) {
        length++;
    }
    return length;
}

static inline void memset(void *ptr, uint8_t value, size_t size) {
    uint8_t *ptrByte = (uint8_t *)ptr;
    for(size_t i = 0; i < size; i++) {
        ptrByte[i] = value;
    }
}

#endif