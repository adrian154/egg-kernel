#include "irq.h"
#include "pic.h"
#include "ioport.h"
#include "idt.h"
#include "gdt.h"

// The mainIRQHandler() is called when an IRQ occurs (irq.asm)
// It dispatches the C IRQ handler for each of the 16 IRQs

extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

// Array of function pointers for the IRQ handlers
// NULL means the handler is not defined and will not be called
IRQHandler IRQHandlers[16] = {
    NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL,
};

// Sets an IRQ handler
void setIRQHandler(int irq, IRQHandler handler) {
    IRQHandlers[irq] = handler;
}

// Removes an IRQ handler
void clearIRQHandler(int irq) {
    IRQHandlers[irq] = NULL;
}

void setupIRQs() {

    // Remap IRQs to 32-48 
    remapPIC(32, 47);

    // Add IRQ handlers to IDT
    addIDTEntry(32, (uint32_t)irq0, GDT_CODE_SELECTOR, IDT_ENTRY_PRESENT | IDT_ENTRY_RING0 | IDT_ENTRY_32BIT | IDT_ENTRY_INTERRUPT_GATE);
    addIDTEntry(33, (uint32_t)irq1, GDT_CODE_SELECTOR, IDT_ENTRY_PRESENT | IDT_ENTRY_RING0 | IDT_ENTRY_32BIT | IDT_ENTRY_INTERRUPT_GATE);
    addIDTEntry(34, (uint32_t)irq2, GDT_CODE_SELECTOR, IDT_ENTRY_PRESENT | IDT_ENTRY_RING0 | IDT_ENTRY_32BIT | IDT_ENTRY_INTERRUPT_GATE);
    addIDTEntry(35, (uint32_t)irq3, GDT_CODE_SELECTOR, IDT_ENTRY_PRESENT | IDT_ENTRY_RING0 | IDT_ENTRY_32BIT | IDT_ENTRY_INTERRUPT_GATE);
    addIDTEntry(36, (uint32_t)irq4, GDT_CODE_SELECTOR, IDT_ENTRY_PRESENT | IDT_ENTRY_RING0 | IDT_ENTRY_32BIT | IDT_ENTRY_INTERRUPT_GATE);
    addIDTEntry(37, (uint32_t)irq5, GDT_CODE_SELECTOR, IDT_ENTRY_PRESENT | IDT_ENTRY_RING0 | IDT_ENTRY_32BIT | IDT_ENTRY_INTERRUPT_GATE);
    addIDTEntry(38, (uint32_t)irq6, GDT_CODE_SELECTOR, IDT_ENTRY_PRESENT | IDT_ENTRY_RING0 | IDT_ENTRY_32BIT | IDT_ENTRY_INTERRUPT_GATE);
    addIDTEntry(39, (uint32_t)irq7, GDT_CODE_SELECTOR, IDT_ENTRY_PRESENT | IDT_ENTRY_RING0 | IDT_ENTRY_32BIT | IDT_ENTRY_INTERRUPT_GATE);
    addIDTEntry(40, (uint32_t)irq8, GDT_CODE_SELECTOR, IDT_ENTRY_PRESENT | IDT_ENTRY_RING0 | IDT_ENTRY_32BIT | IDT_ENTRY_INTERRUPT_GATE);
    addIDTEntry(41, (uint32_t)irq9, GDT_CODE_SELECTOR, IDT_ENTRY_PRESENT | IDT_ENTRY_RING0 | IDT_ENTRY_32BIT | IDT_ENTRY_INTERRUPT_GATE);
    addIDTEntry(42, (uint32_t)irq10, GDT_CODE_SELECTOR, IDT_ENTRY_PRESENT | IDT_ENTRY_RING0 | IDT_ENTRY_32BIT | IDT_ENTRY_INTERRUPT_GATE);
    addIDTEntry(43, (uint32_t)irq11, GDT_CODE_SELECTOR, IDT_ENTRY_PRESENT | IDT_ENTRY_RING0 | IDT_ENTRY_32BIT | IDT_ENTRY_INTERRUPT_GATE);
    addIDTEntry(44, (uint32_t)irq12, GDT_CODE_SELECTOR, IDT_ENTRY_PRESENT | IDT_ENTRY_RING0 | IDT_ENTRY_32BIT | IDT_ENTRY_INTERRUPT_GATE);
    addIDTEntry(45, (uint32_t)irq13, GDT_CODE_SELECTOR, IDT_ENTRY_PRESENT | IDT_ENTRY_RING0 | IDT_ENTRY_32BIT | IDT_ENTRY_INTERRUPT_GATE);
    addIDTEntry(46, (uint32_t)irq14, GDT_CODE_SELECTOR, IDT_ENTRY_PRESENT | IDT_ENTRY_RING0 | IDT_ENTRY_32BIT | IDT_ENTRY_INTERRUPT_GATE);
    addIDTEntry(47, (uint32_t)irq15, GDT_CODE_SELECTOR, IDT_ENTRY_PRESENT | IDT_ENTRY_RING0 | IDT_ENTRY_32BIT | IDT_ENTRY_INTERRUPT_GATE);

}

// Called when an IRQ occurs (irq.asm)
// Dispatches an IRQ-specific handler
void mainIRQHandler(struct IRQFrame *frame) {

    IRQHandler handler = IRQHandlers[frame->interruptNumber - 32];
    if(handler != NULL) {
        handler(frame);
    }

    // If the IRQ was on interrupts 8-15 (remapped to 40-47), we need to send EOI to the slave PIC
    if(frame->interruptNumber > 39) {
        outb(PIC_SLAVE_COMMAND, PIC_EOI);
    }

    // Always need to send EOI to master PIC
    outb(PIC_MASTER_COMMAND, PIC_EOI);

}