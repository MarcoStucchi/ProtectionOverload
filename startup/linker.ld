SECTIONS {
    .isr_vector : { KEEP(*(.isr_vector)) } > FLASH
    .stack : { *(.stack) } > RAM
}
