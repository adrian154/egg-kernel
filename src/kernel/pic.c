#include "pic.h"
#include "ioport.h"

// Remap PICs so that IRQs don't coincide with exceptions (irq.c)

void remapPIC(uint8_t masterOffset, uint8_t slaveOffset) {

    // configure master PIC
    outb(PIC_MASTER_COMMAND, PIC_INITIALIZE);   // start configuring pic
    outb(PIC_MASTER_DATA, masterOffset);        // ICW2: interupt vector offset
    outb(PIC_MASTER_DATA, 4);                   // ICW3: set bit #2 to indicate that there's a slave PIC on IRQ 2
    outb(PIC_MASTER_DATA, PIC_ICW4_8086);       // ICW4: mode of operation (8086)

    // configure slave PIC
    outb(PIC_SLAVE_COMMAND, PIC_INITIALIZE);
    outb(PIC_SLAVE_DATA, slaveOffset);          
    outb(PIC_SLAVE_DATA, 2);                    // ICW3: for slave PICs this is the IRQ number to operate on
    outb(PIC_SLAVE_DATA, PIC_ICW4_8086);
    
}