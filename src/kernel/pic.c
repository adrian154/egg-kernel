#include "pic.h"
#include "ioport.h"

// Remap PICs so that IRQs don't coincide with exceptions (irq.c)

void remapPIC(uint8_t masterOffset, uint8_t slaveOffset) {

    // configure master PIC
    outb(PIC_MASTER_COMMAND, PIC_INITIALIZE);   // ICW1 = initialize command
    outb(PIC_MASTER_DATA, masterOffset);        // ICW2 = interupt vector offset
    outb(PIC_MASTER_DATA, 4);                   // ICW3 = slave IRQ (for master)
    outb(PIC_MASTER_DATA, PIC_ICW4_8086);       // Use  8086 mode (we're not on an MCS-80 :)

    // configure slave PIC
    outb(PIC_SLAVE_COMMAND, PIC_INITIALIZE);
    outb(PIC_SLAVE_DATA, slaveOffset);
    outb(PIC_SLAVE_DATA, 2);                    // ICW3 = slave cascade identity (for slave)
    outb(PIC_SLAVE_DATA, PIC_ICW4_8086);
    
}