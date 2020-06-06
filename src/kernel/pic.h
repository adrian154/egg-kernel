#ifndef __PIC_H
#define __PIC_H

#include "kernel.h"

// PIC IO ports
#define PIC_MASTER_COMMAND      0x20
#define PIC_MASTER_DATA         0x21
#define PIC_SLAVE_COMMAND       0xA0
#define PIC_SLAVE_DATA          0xA1

// Commands
#define PIC_EOI                 0x20    // End interrupt, should be sent by IRQ handler
#define PIC_INITIALIZE          0x11    // After sending, PICs will wait for 3 bytes (ICW2-4) on data ports
                                        // ICW2 = offset
                                        // ICW3 = (master pic: which IRQ the slave will cascade on) or (slave pic: which IRQ to cascade on)
                                        // ICW4 = mode of operation

#define PIC_ICW4_8086               0b00000001      // 8086 mode (default: MCS-80 mode)
#define PIC_ICW4_AUTO_EOI           0x0b000010      // Auto-EOI (default: normal EOI)
#define PIC_ICW4_BUFFERED_SLAVE     0b00001000      // Buffered move for slave (default: non-buffered)
#define PIC_ICW4_BUFFERED_MASTER    0b00001100      // Buffered mode for master (default: non-buffered)
#define PIC_ICW4_SFNM               0b00010000      // "Special fully nested mode" (default: unspecial fully nested mode)

#endif