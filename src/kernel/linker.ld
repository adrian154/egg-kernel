OUTPUT_FORMAT(elf32-i386)
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

    .padding : {
        BYTE(0xFF);
        . = ALIGN(512);
    } = 0xFF

}

kernelPhysicalEnd = .;