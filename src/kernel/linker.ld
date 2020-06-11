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

    /* Add padding to placate VirtualBox and bochs */
    .padding : {
        BYTE(0xAA);
        . = ALIGN(512);
    } =0xEE

}

kernelPhysicalEnd = .;