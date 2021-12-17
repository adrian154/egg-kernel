MEMORY_MAP_OFFSET equ 0x8200    ; Offset at which the memory map will be stored
KERNEL_SIZE_SECTORS equ 57      ; Size of the kernel in sectors
KERNEL_LOAD_OFFSET equ 0x8400   ; Segment-offset where the kernel will be loaded in memory
KERNEL_LOAD_SEGMENT equ 0x0000  ; (...)
KERNEL_START_SECTOR equ 3       ; First sector of kernel on disk