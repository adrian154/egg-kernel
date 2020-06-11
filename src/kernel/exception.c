#include "exception.h"
#include "terminal.h"
#include "idt.h"
#include "gdt.h"

// Exception messages
const char *exceptionMessages[32] = {
    "Division by Zero",
    "Debug Exception",
    "Non-Maskable Interrupt",
    "Breakpoint",
    "Overflow",
    "BOUND Range Exceeded",
    "Invalid Opcode",
    "Device Not Available",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Invalid TSS",
    "Segment Not Present",
    "Stack-Segment Fault",
    "General Protection Fault",
    "Page Fault",
    "Reserved (15)",
    "x87 Floating-Point Exception",
    "Alignment Check",
    "Machine Check",
    "SIMD Floating-Point Exception",
    "Virtualization Exception",
    "Reserved (21)",
    "Reserved (22)",
    "Reserved (23)",
    "Reserved (24)",
    "Reserved (25)",
    "Reserved (26)",
    "Reserved (27)",
    "Reserved (28)",
    "Reserved (29)",
    "Reserved (30)",
    "Reserved (31)"
};

// Eventually, when usermode + multitasking is implemented, exceptions will be handled differently
// For now, treat everything as fatal since there's only the kernel running and nothing to fall back on

void exceptionHandler(struct ExceptionFrame *frame) {

    terminalColor = makeColor(TERMINAL_COLOR_RED, TERMINAL_COLOR_BLACK);
    print("exception occurred!\n");
    print("DS="); printHexInt(frame->DS); print(", ES="); printHexInt(frame->ES); print(", FS="); printHexInt(frame->FS); print(", GS="); printHexInt(frame->GS);
    print("\nEDI="); printHexInt(frame->EDI); print(", ESI="); printHexInt(frame->ESI); print(", EBP="); printHexInt(frame->kernelEBP); print(", ESP="); printHexInt(frame->kernelESP);
    print("\nEBX="); printHexInt(frame->EBX); print(", EDX="); printHexInt(frame->EDX); print(", ECX="); printHexInt(frame->ECX); print(", EAX="); printHexInt(frame->EAX); 
    print("\nINT_NO="); printHexInt(frame->interruptNumber); print(", ERR_CODE="); printHexInt(frame->errorCode);
    print("\nEFLAGS="); printHexInt(frame->EFLAGS); print(", CS="); printHexInt(frame->CS); print(", EIP="); printHexInt(frame->EIP);
    print("\nUserSS="); printHexInt(frame->userSS); print(", UserESP="); printHexInt(frame->userESP);
    print("\nexception name="); print(exceptionMessages[frame->interruptNumber]);

    // Special handler for page faults
    if(frame->interruptNumber == PAGE_FAULT_EXCEPTION) {

        putChar('\n');

        // debug: CR2 contains the virtual address of the illegal access
        uint32_t addr;
        __asm__ __volatile__("mov %%cr2, %0" : "=r" (addr) :: "eax");
        
        if(frame->errorCode & 0b100) {
            print("user ");
        } else {
            print("kernel ");
        }
        if(frame->errorCode & 0b010) {
            print("wrote to ");
        } else {
            print("read from ");
        }
        if(frame->errorCode & 0b001) {
            print("a present page");
        } else {
            print("a non-present page");
        }

    }

    for(;;);

}

void setupExceptionHandlers() {

    // Set up each exception handler as an interrupt gate
    addIDTEntry(0, (uint32_t)isr0, GDT_CODE_SELECTOR, IDT_ENTRY_PRESENT | IDT_ENTRY_RING0 | IDT_ENTRY_32BIT | IDT_ENTRY_INTERRUPT_GATE);
    addIDTEntry(1, (uint32_t)isr1, GDT_CODE_SELECTOR, IDT_ENTRY_PRESENT | IDT_ENTRY_RING0 | IDT_ENTRY_32BIT | IDT_ENTRY_INTERRUPT_GATE);
    addIDTEntry(2, (uint32_t)isr2, GDT_CODE_SELECTOR, IDT_ENTRY_PRESENT | IDT_ENTRY_RING0 | IDT_ENTRY_32BIT | IDT_ENTRY_INTERRUPT_GATE);
    addIDTEntry(3, (uint32_t)isr3, GDT_CODE_SELECTOR, IDT_ENTRY_PRESENT | IDT_ENTRY_RING0 | IDT_ENTRY_32BIT | IDT_ENTRY_INTERRUPT_GATE);
    addIDTEntry(4, (uint32_t)isr4, GDT_CODE_SELECTOR, IDT_ENTRY_PRESENT | IDT_ENTRY_RING0 | IDT_ENTRY_32BIT | IDT_ENTRY_INTERRUPT_GATE);
    addIDTEntry(5, (uint32_t)isr5, GDT_CODE_SELECTOR, IDT_ENTRY_PRESENT | IDT_ENTRY_RING0 | IDT_ENTRY_32BIT | IDT_ENTRY_INTERRUPT_GATE);
    addIDTEntry(6, (uint32_t)isr6, GDT_CODE_SELECTOR, IDT_ENTRY_PRESENT | IDT_ENTRY_RING0 | IDT_ENTRY_32BIT | IDT_ENTRY_INTERRUPT_GATE);
    addIDTEntry(7, (uint32_t)isr7, GDT_CODE_SELECTOR, IDT_ENTRY_PRESENT | IDT_ENTRY_RING0 | IDT_ENTRY_32BIT | IDT_ENTRY_INTERRUPT_GATE);
    addIDTEntry(8, (uint32_t)isr8, GDT_CODE_SELECTOR, IDT_ENTRY_PRESENT | IDT_ENTRY_RING0 | IDT_ENTRY_32BIT | IDT_ENTRY_INTERRUPT_GATE);
    addIDTEntry(9, (uint32_t)isr9, GDT_CODE_SELECTOR, IDT_ENTRY_PRESENT | IDT_ENTRY_RING0 | IDT_ENTRY_32BIT | IDT_ENTRY_INTERRUPT_GATE);
    addIDTEntry(10, (uint32_t)isr10, GDT_CODE_SELECTOR, IDT_ENTRY_PRESENT | IDT_ENTRY_RING0 | IDT_ENTRY_32BIT | IDT_ENTRY_INTERRUPT_GATE);
    addIDTEntry(11, (uint32_t)isr11, GDT_CODE_SELECTOR, IDT_ENTRY_PRESENT | IDT_ENTRY_RING0 | IDT_ENTRY_32BIT | IDT_ENTRY_INTERRUPT_GATE);
    addIDTEntry(12, (uint32_t)isr12, GDT_CODE_SELECTOR, IDT_ENTRY_PRESENT | IDT_ENTRY_RING0 | IDT_ENTRY_32BIT | IDT_ENTRY_INTERRUPT_GATE);
    addIDTEntry(13, (uint32_t)isr13, GDT_CODE_SELECTOR, IDT_ENTRY_PRESENT | IDT_ENTRY_RING0 | IDT_ENTRY_32BIT | IDT_ENTRY_INTERRUPT_GATE);
    addIDTEntry(14, (uint32_t)isr14, GDT_CODE_SELECTOR, IDT_ENTRY_PRESENT | IDT_ENTRY_RING0 | IDT_ENTRY_32BIT | IDT_ENTRY_INTERRUPT_GATE);
    addIDTEntry(15, (uint32_t)isr15, GDT_CODE_SELECTOR, IDT_ENTRY_PRESENT | IDT_ENTRY_RING0 | IDT_ENTRY_32BIT | IDT_ENTRY_INTERRUPT_GATE);
    addIDTEntry(16, (uint32_t)isr16, GDT_CODE_SELECTOR, IDT_ENTRY_PRESENT | IDT_ENTRY_RING0 | IDT_ENTRY_32BIT | IDT_ENTRY_INTERRUPT_GATE);
    addIDTEntry(17, (uint32_t)isr17, GDT_CODE_SELECTOR, IDT_ENTRY_PRESENT | IDT_ENTRY_RING0 | IDT_ENTRY_32BIT | IDT_ENTRY_INTERRUPT_GATE);
    addIDTEntry(18, (uint32_t)isr18, GDT_CODE_SELECTOR, IDT_ENTRY_PRESENT | IDT_ENTRY_RING0 | IDT_ENTRY_32BIT | IDT_ENTRY_INTERRUPT_GATE);
    addIDTEntry(19, (uint32_t)isr19, GDT_CODE_SELECTOR, IDT_ENTRY_PRESENT | IDT_ENTRY_RING0 | IDT_ENTRY_32BIT | IDT_ENTRY_INTERRUPT_GATE);
    addIDTEntry(20, (uint32_t)isr20, GDT_CODE_SELECTOR, IDT_ENTRY_PRESENT | IDT_ENTRY_RING0 | IDT_ENTRY_32BIT | IDT_ENTRY_INTERRUPT_GATE);
    addIDTEntry(21, (uint32_t)isr21, GDT_CODE_SELECTOR, IDT_ENTRY_PRESENT | IDT_ENTRY_RING0 | IDT_ENTRY_32BIT | IDT_ENTRY_INTERRUPT_GATE);
    addIDTEntry(22, (uint32_t)isr22, GDT_CODE_SELECTOR, IDT_ENTRY_PRESENT | IDT_ENTRY_RING0 | IDT_ENTRY_32BIT | IDT_ENTRY_INTERRUPT_GATE);
    addIDTEntry(23, (uint32_t)isr23, GDT_CODE_SELECTOR, IDT_ENTRY_PRESENT | IDT_ENTRY_RING0 | IDT_ENTRY_32BIT | IDT_ENTRY_INTERRUPT_GATE);
    addIDTEntry(24, (uint32_t)isr24, GDT_CODE_SELECTOR, IDT_ENTRY_PRESENT | IDT_ENTRY_RING0 | IDT_ENTRY_32BIT | IDT_ENTRY_INTERRUPT_GATE);
    addIDTEntry(25, (uint32_t)isr25, GDT_CODE_SELECTOR, IDT_ENTRY_PRESENT | IDT_ENTRY_RING0 | IDT_ENTRY_32BIT | IDT_ENTRY_INTERRUPT_GATE);
    addIDTEntry(26, (uint32_t)isr26, GDT_CODE_SELECTOR, IDT_ENTRY_PRESENT | IDT_ENTRY_RING0 | IDT_ENTRY_32BIT | IDT_ENTRY_INTERRUPT_GATE);
    addIDTEntry(27, (uint32_t)isr27, GDT_CODE_SELECTOR, IDT_ENTRY_PRESENT | IDT_ENTRY_RING0 | IDT_ENTRY_32BIT | IDT_ENTRY_INTERRUPT_GATE);
    addIDTEntry(28, (uint32_t)isr28, GDT_CODE_SELECTOR, IDT_ENTRY_PRESENT | IDT_ENTRY_RING0 | IDT_ENTRY_32BIT | IDT_ENTRY_INTERRUPT_GATE);
    addIDTEntry(29, (uint32_t)isr29, GDT_CODE_SELECTOR, IDT_ENTRY_PRESENT | IDT_ENTRY_RING0 | IDT_ENTRY_32BIT | IDT_ENTRY_INTERRUPT_GATE);
    addIDTEntry(30, (uint32_t)isr30, GDT_CODE_SELECTOR, IDT_ENTRY_PRESENT | IDT_ENTRY_RING0 | IDT_ENTRY_32BIT | IDT_ENTRY_INTERRUPT_GATE);
    addIDTEntry(31, (uint32_t)isr31, GDT_CODE_SELECTOR, IDT_ENTRY_PRESENT | IDT_ENTRY_RING0 | IDT_ENTRY_32BIT | IDT_ENTRY_INTERRUPT_GATE);
    
}