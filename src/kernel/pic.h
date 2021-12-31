#ifndef __PIC_H
#define __PIC_H

#include "kernel.h"

// PIC IO ports
#define PIC_MASTER_COMMAND      0x20
#define PIC_MASTER_DATA         0x21
#define PIC_SLAVE_COMMAND       0xA0
#define PIC_SLAVE_DATA          0xA1

// Commands
#define PIC_EOI                 0x20
#define PIC_INITIALIZE          0x11 
#define PIC_ICW4_8086           0x1

extern void remapPIC(uint8_t masterOffset, uint8_t slaveOffset);

#endif