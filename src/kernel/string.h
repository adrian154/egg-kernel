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

// syntax is not standard, but i think it makes more sense like this
static inline void memcpy(void *src, void *dest, size_t size) {

    uint8_t *srcPtr = (uint8_t *)src;
    uint8_t *destPtr = (uint8_t *)dest;
    for(size_t i = 0; i < size; i++) {
        destPtr[i] = srcPtr[i];
    }

}

#endif