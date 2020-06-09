OUTPUT_FORMAT("binary")
ENTRY(start)

. =  0x00100000;

kernelPhysicalStart = .;

SECTIONS {

    .text : {
        *(.text)
    }

    .rodata : {
        *(.rodata)
    }

    .data : {
        *(.data)
    }

    .bss : {
        *(.bss)
    }

}

kernelPhysicalEnd = .;