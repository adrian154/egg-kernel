OUTPUT_FORMAT("binary")
ENTRY(start)

. = 0x7E00;

SECTIONS {

    .text : {
        *(.text)
    }

}