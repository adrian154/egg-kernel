OUTPUT_FORMAT("binary")
ENTRY(start)

. =  0x00100000;

kernelPhysicalStart = .;

SECTIONS {

    .start : {
        *(.start)
    }

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