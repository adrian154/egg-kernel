#ifndef __PRINT_H
#define __PRINT_H

#include <stdarg.h>
#include <stdbool.h>
#include "terminal.h"

// This is a NON-STANDARD printf()!!
extern void printf(const char *format, ...);
extern void print(const char *str);

#endif