#ifndef __IOPORT_H
#define __IOPORT_H

#include "kernel.h"

// Functions to interface with IO ports from C (ioport.asm)
extern uint8_t inb(uint16_t port);
extern void outb(uint16_t port, uint8_t data);

#endif