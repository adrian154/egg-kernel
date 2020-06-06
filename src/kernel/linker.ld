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

    .padding : {
        BYTE(0x00);
        . = ALIGN(512);
    } =0xEE

}

kernelPhysicalEnd = .;