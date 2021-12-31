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

    /* Add padding to placate VirtualBox and bochs */
    .padding : {
        . = ALIGN(512);
    } =0xEE

}

kernelPhysicalEnd = .;