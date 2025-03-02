/* Memory layout for lm3s6965evb */
MEMORY
{
    FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 256K
    RAM (rwx)  : ORIGIN = 0x20000000, LENGTH = 64K
}

/* Stack size */
STACK_SIZE = 4K;
HEAP_SIZE = 4K;

/* Define sections */
SECTIONS
{
    .isr_vector :
    {
        . = ALIGN(4);
        KEEP(*(.isr_vector))
        . = ALIGN(4);
    } > FLASH

    .text :
    {
        . = ALIGN(4);
        *(.text)
        *(.text*)
        *(.rodata)
        *(.rodata*)
        . = ALIGN(4);
    } > FLASH

    .ARM.extab :
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > FLASH

    .ARM.exidx :
    {
        PROVIDE_HIDDEN(__exidx_start = .);
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
        PROVIDE_HIDDEN(__exidx_end = .);
    } > FLASH

    _data_loadaddr = LOADADDR(.data);

    .data :
    {
        . = ALIGN(4);
        __data_start__ = .;
        *(.data)
        *(.data*)
        . = ALIGN(4);
        __data_end__ = .;
    } > RAM AT > FLASH

    .bss :
    {
        . = ALIGN(4);
        __bss_start__ = .;
        *(.bss)
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        __bss_end__ = .;
        _end = .;    /* Define _end symbol */
        end = .;     /* Alternative name */
    } > RAM

    .heap :
    {
        . = ALIGN(8);
        __heap_start__ = .;
        . = . + HEAP_SIZE;
        __heap_end__ = .;
    } > RAM

    /* Stack allocation */
    .stack (NOLOAD) :
    {
        . = ALIGN(8);
        . = . + STACK_SIZE;
        . = ALIGN(8);
        _stack_top = .;
    } > RAM
}

/* PROVIDE(_stack_top = ORIGIN(RAM) + LENGTH(RAM)); */
ENTRY(Reset_Handler)