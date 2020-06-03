#ifndef __STRING_H
#define __STRING_H

#include "kernel.h"

inline unsigned int strlen(const char *str) {
    unsigned int length = 0;
    for(const char *ptr = str; ptr[length] != '\0'; ptr++) {
        length++;
    }
    return length;
}

#endif